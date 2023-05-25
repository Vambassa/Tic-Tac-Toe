cmake_minimum_required(VERSION 3.25)

project(Tic-Tac-Toe)

add_executable(tic-tac-toe main.cpp)

if ( UNIX )
    if (Debug)
        add_definitions(" -O0 -g -Wall -std=c++17 ")
    else ()
        add_definitions(" -O3 -Wall -Werror -std=c++17 ")
    endif (Debug)
    add_definitions(" -pedantic -pedantic-errors -W")
endif ( UNIX )
