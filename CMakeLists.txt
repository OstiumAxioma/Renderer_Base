cmake_minimum_required(VERSION 3.12)

project(OpenGL_Window)

set(CMAKE_CXX_STANDARD 17)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/header
    SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/include
)

link_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/lib
)

add_executable(OpenGL_Window
    src/main.cpp
    src/glad.c
)

target_link_libraries(OpenGL_Window glfw3.lib)
