# cmake_minimum_required(VERSION <specify CMake version here>)
project(test C)

set(CMAKE_C_STANDARD 11)

#add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
#add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
#add_compile_options(/source-charset:utf-8)
#add_compile_options(/execution-charset:utf-8)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:WinMainCRTStartup")

add_executable(
        test_server
        test_server.c
)

add_executable(
        test_desktop
        test_desktop.c
        ../client/td_screen.c
        ../client/td_screen.h
)

add_executable(
        test_client
        test_client.c
        ../client/td_screen.c
        ../client/td_screen.h
        ../core/td_rdp.c
        ../core/td_rdp.h
)