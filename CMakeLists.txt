cmake_minimum_required(VERSION 3.10)

project(liblucu)

option(DEBUG "Build a debug version" OFF)

add_compile_options(-std=c23 -Wall -Wextra -Werror -pedantic -pedantic-errors -Wfloat-equal -Wundef -Wshadow -Wcast-align=strict -Wstrict-overflow=5 -Wcast-qual -Wswitch-default -Wswitch-enum -Wconversion -Warith-conversion -Wformat=2 -Wformat-overflow=2 -Wformat-nonliteral -Wformat-signedness -Wformat-truncation=2 -Wmissing-include-dirs)

if (${DEBUG})
	add_compile_options(-Og -ggdb)
else()
	add_compile_options(-O3)
endif()

include_directories(${CMAKE_SOURCE_DIR}/include/)

add_subdirectory(src)
add_subdirectory(tests)
