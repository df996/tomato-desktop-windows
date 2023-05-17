# cmake_minimum_required(VERSION <specify CMake version here>)
project(tomato_desktop_windows C)

set(CMAKE_C_STANDARD 11)

add_executable(
        tomato_desktop_windows
        src/tomato-desktop.c
        src/core/td_thread_pool.c
        src/core/td_thread_pool.h
        src/client/td_screen.c
        src/client/td_screen.h
        src/network/td_connect.c src/network/td_connect.h)