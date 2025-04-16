cmake_minimum_required(VERSION 3.10)
project(Cube)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

add_library(glad STATIC glad/glad.c)

add_executable(cube 
    src/main.cpp
    src/window/GlfwEngineWindow.cpp
    src/component/CubeMesh.cpp
)
target_link_libraries(cube glfw OpenGL::GL glad)
