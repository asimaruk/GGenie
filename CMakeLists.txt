cmake_minimum_required(VERSION 3.23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # clang-tidy needs compile_commands.json
project(GGenie)

configure_file(
    "${CMAKE_SOURCE_DIR}/demo/include/config.h.in"
    "${CMAKE_BINARY_DIR}/config.h"
    @ONLY
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(Catch2 3 REQUIRED)
find_package(freetype REQUIRED)

find_program(CLANG_FORMAT "clang-format")
find_program(CLANG_TIDY "clang-tidy")
find_program(CLANG_APPLY_REPLACEMENTS "clang-apply-replacements")
find_program(GIT "git")

add_library(glad STATIC glad/glad.c)

file(GLOB_RECURSE ENGINE_SOURCES 
    "engine/src/**/*.cpp"
    "engine/include/**/*.hpp"
    "engine/include/**/*.h"
)

file(GLOB_RECURSE DEMO_SOURCES "demo/src/**/*.cpp")

file(GLOB_RECURSE ENGINE_TESTS 
    "engine/test/**/*.test.cpp"
)

if(CLANG_FORMAT)
    message("Adding format target")
    add_custom_target(
        format
        COMMAND ${CLANG_FORMAT} -i --style=file ${ENGINE_SOURCES} ${ENGINE_TESTS} ${DEMO_SOURCES}
        COMMENT "Running clang-format on C++ files"
    )
else()
    message("clang-format not found, skip format target")
endif()

if(CLANG_FORMAT AND GIT)
    message("Adding changed-format target")
    add_custom_target(
        changed-format
        COMMAND ${GIT} clang-format --extensions cpp,hpp,h,cc,cxx -f
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Running git clang-format on changed C++ files"
    )
else()
    message("clang-format or git not found, skip changed-format target")
endif()

if(CLANG_TIDY)
    message("Adding tidy target")
    if (CMAKE_BUILD_TYPE STREQUAL "Release")
        set(CMAKE_CXX_CLANG_TIDY ${CLANG_TIDY})
    endif()
    add_custom_target(
        tidy
        COMMAND ${CLANG_TIDY} -p build/${CMAKE_BUILD_TYPE} ${ENGINE_SOURCES} ${DEMO_SOURCES}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Running git clang-tidy on C++ files"
    )

    add_custom_target(
        tidy-report
        COMMAND ${CLANG_TIDY} -p build/${CMAKE_BUILD_TYPE} ${ENGINE_SOURCES} ${DEMO_SOURCES} --export-fixes=tidy-report.yaml
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Running git clang-tidy with fixes reported in tidy-report.yaml"
    )

    if(CLANG_APPLY_REPLACEMENTS)
        add_custom_target(
            tidy-fix
            COMMAND ${CLANG_APPLY_REPLACEMENTS} --style=file --ignore-insert-conflict .
            DEPENDS tidy-report
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            COMMENT "Running clang-apply-replacements from tidy-report.yaml"
        )
    endif()
else()
    message("clang-tidy not found, skip tidy target")
endif()

message("Building with CMake version: ${CMAKE_VERSION}")

add_library(ggenie_deps INTERFACE)
target_link_libraries(ggenie_deps INTERFACE 
    OpenGL::GL
    glfw
    glm::glm
    glad
    Freetype::Freetype
)

add_library(ggenie_includes INTERFACE)
target_include_directories(ggenie_includes INTERFACE
    ${CMAKE_SOURCE_DIR}/glad
    ${CMAKE_SOURCE_DIR}/engine/include
)

add_executable(GGenie 
    ${CMAKE_SOURCE_DIR}/demo/src/main.cpp
    ${ENGINE_SOURCES}
)
target_link_libraries(GGenie 
    ggenie_deps 
    ggenie_includes
)
target_include_directories(GGenie PUBLIC # to include config.h
    "${PROJECT_BINARY_DIR}"
)

message("ENGINE_TESTS", ${ENGINE_TESTS})
add_executable(tests 
    ${ENGINE_SOURCES}
    ${ENGINE_TESTS}
)
target_link_libraries(tests 
    PRIVATE Catch2::Catch2WithMain
    ggenie_deps
    ggenie_includes
)

add_dependencies(GGenie changed-format)

file(COPY "engine/resources/"
    DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/resources"
)
