cmake_minimum_required(VERSION 3.21...3.31)

project(
	liblucu
	VERSION 1.3.0
	DESCRIPTION "Lucy's Useful C Utils"
	LANGUAGES C
)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

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
	find_package(Criterion REQUIRED)
	add_subdirectory(tests)
endif()

export(TARGETS lucu NAMESPACE lucu:: FILE lucuTargets.cmake)
export(PACKAGE lucu)
