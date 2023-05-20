# cmake_minimum_required(VERSION <specify CMake version here>)
project(tomato_desktop_windows C)

#add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
#add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
#add_compile_options(/utf-8)
#add_compile_options(/source-charset:utf-8)
#add_compile_options(/execution-charset:utf-8)

#add_compile_options(/source-charset:utf-8)
#add_compile_options(/execution-charset:utf-8)

#add_definitions("-DUNICODE" "-D_UNICODE")

set(CMAKE_C_STANDARD 11)
add_compile_options(/utf-8)
add_definitions(-DUNICODE -D_UNICODE)

add_executable(
        tomato_desktop_windows
        src/tomato-desktop.c
        src/core/td_thread_pool.c
        src/core/td_thread_pool.h
        src/client/td_screen.c
        src/client/td_screen.h
        src/core/td_rdp.c
        src/core/td_rdp.h
        src/core/td_connection.c
        src/core/td_connection.h
)

ADD_SUBDIRECTORY(${PROJECT_SOURCE_DIR}/src/test)