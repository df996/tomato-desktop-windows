# cmake_minimum_required(VERSION <specify CMake version here>)
project(tomato_desktop_windows_test C)

set(CMAKE_C_STANDARD 11)

#add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
#add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
#add_compile_options(/source-charset:utf-8)
#add_compile_options(/execution-charset:utf-8)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:WinMainCRTStartup")

add_executable(
        tomato_desktop_windows_test
        test_server.c
)
