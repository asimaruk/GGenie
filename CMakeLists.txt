cmake_minimum_required(VERSION 3.23)
project(GGenie)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)  # Обязательное требование
set(CMAKE_CXX_EXTENSIONS OFF)  # Отключаем расширения компилятора (например, GNU-расширения)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

find_program(CLANG_FORMAT "clang-format")
find_program(CLANG_TIDY "clang-tidy")
find_program(GIT "git")

add_library(glad STATIC glad/glad.c)

file(GLOB_RECURSE ENGINE_SOURCES 
    "engine/src/**/*.cpp"
)

if(CLANG_FORMAT)
    message("Adding format target")
    add_custom_target(
        format
        COMMAND ${CLANG_FORMAT} -i --style=file ${ENGINE_SOURCES}
        COMMENT "Running clang-format on C++ sources"
    )
else()
    message("Missing clang-format, format target not added")
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
    message("Missing clang-format or git, changed-format target not added")
endif()

if(CLANG_TIDY)
    message("clang-tidy found")
    set(CMAKE_CXX_CLANG_TIDY ${CLANG_TIDY})
else()
    message("Missing clang-tidy, lint target not added")
endif()

message("Building with CMake version: ${CMAKE_VERSION}")

add_executable(GGenie 
    ${CMAKE_SOURCE_DIR}/demo/src/main.cpp
    ${ENGINE_SOURCES}
)

target_include_directories(GGenie PRIVATE
    ${CMAKE_SOURCE_DIR}/glad
    ${CMAKE_SOURCE_DIR}/engine/include
)

target_link_libraries(GGenie 
    OpenGL::GL 
    glfw 
    glm::glm 
    glad
)

add_dependencies(GGenie changed-format)
