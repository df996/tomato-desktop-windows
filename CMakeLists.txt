# cmake_minimum_required(VERSION <specify CMake version here>)
project(tomato_desktop_windows C)

set(CMAKE_C_STANDARD 11)

add_executable(
        tomato_desktop_windows
        src/tomato-desktop.c
)