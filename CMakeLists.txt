cmake_minimum_required(VERSION 3.12)

project(OpenGL_Window)

set(CMAKE_CXX_STANDARD 17)

include_directories(
    SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/header
    SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/include
)

link_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/lib
)

aux_source_directory(${CMAKE_SOURCE_DIR}/src SOURCES)
aux_source_directory(${CMAKE_SOURCE_DIR}/header HEADERS)
add_subdirectory(wrapper)

add_executable(OpenGL_Window
    ${SOURCES}
    ${HEADERS}
)

target_link_libraries(OpenGL_Window glfw3.lib wrapper)
