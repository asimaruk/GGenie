cmake_minimum_required(VERSION 3.10)
project(Engine3d)

set(CMAKE_CXX_STANDARD 23)  # Или 14, 20, 23
set(CMAKE_CXX_STANDARD_REQUIRED ON)  # Обязательное требование
set(CMAKE_CXX_EXTENSIONS OFF)  # Отключаем расширения компилятора (например, GNU-расширения)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

add_library(glad STATIC glad/glad.c)

add_executable(engine3d 
    src/main.cpp
    src/window/GlfwEngineWindow.cpp
    src/component/CubeMesh.cpp
    src/component/Shader.cpp
)
target_link_libraries(engine3d glfw OpenGL::GL glad)
