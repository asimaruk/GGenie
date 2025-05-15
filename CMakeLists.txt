cmake_minimum_required(VERSION 3.23)
project(Engine3d)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)  # Обязательное требование
set(CMAKE_CXX_EXTENSIONS OFF)  # Отключаем расширения компилятора (например, GNU-расширения)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

add_library(glad STATIC glad/glad.c)

file(GLOB_RECURSE ENGINE_SOURCES 
    "engine/src/**/*.cpp"
)

message("Building with CMake version: ${CMAKE_VERSION}")

add_executable(engine3d 
    ${CMAKE_SOURCE_DIR}/demo/src/main.cpp
    ${ENGINE_SOURCES}
)
target_include_directories(engine3d PRIVATE
    ${CMAKE_SOURCE_DIR}/glad
    ${CMAKE_SOURCE_DIR}/engine/include
)
target_link_libraries(engine3d OpenGL::GL glfw glm::glm glad)
