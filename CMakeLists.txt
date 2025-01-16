cmake_minimum_required(VERSION 3.10)

project(liblucu)

add_compile_options(-std=c23 -Wall -Wextra -Werror -pedantic -pedantic-errors -Wfloat-equal -Wundef -Wshadow -Wstrict-overflow=5 -Wcast-qual -Wswitch-default -Wswitch-enum -Wconversion -Wformat=2 -Wformat-nonliteral -Wformat-signedness -Wmissing-include-dirs)

include_directories(${CMAKE_SOURCE_DIR}/include/)

add_subdirectory(src)
add_subdirectory(tests)
