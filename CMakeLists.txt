cmake_minimum_required(VERSION 3.21...3.31)

project(
	liblucu
	VERSION 0.1.0
	DESCRIPTION "Lucy's Useful C Utils"
	LANGUAGES C
)

if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
	set(CMAKE_C_STANDARD 23)
	set(CMAKE_EXTENTIONS OFF)
	set_property(GLOBAL PROPERTY USE_FOLDERS ON)
	include(CTest)
	find_package(Doxygen)
	if (Doxygen_FOUND)
		add_subdirectory(docs)
	else()
		message(STATUS "Doxygen not found, not building docs")
	endif()
endif()

add_subdirectory(src)

if ((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME OR LIBLUCU_BUILD_TESTING) AND BUILD_TESTING)
	add_subdirectory(tests)
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	add_compile_options(-std=c23 -Wall -Wextra -Werror -pedantic -pedantic-errors -Wfloat-equal -Wundef -Wshadow -Wstrict-overflow=5 -Wcast-qual -Wswitch-default -Wswitch-enum -Wconversion -Wformat=2 -Wformat-nonliteral -Wformat-signedness -Wmissing-include-dirs)
endif()
