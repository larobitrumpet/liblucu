file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/include/lucu/*.h")

add_library(lucu vector.c option.c cache.c ${HEADER_LIST})
target_include_directories(
	lucu PUBLIC
	$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include>
)

source_group(
	TREE "${PROJECT_SOURCE_DIR}/include"
	PREFIX "Header Files"
	FILES ${HEADER_LIST}
)

target_sources(
	lucu PUBLIC
	FILE_SET HEADERS
	BASE_DIRS ${PROJECT_SOURCE_DIR}/include
	FILES ${HEADER_LIST}
)

install(
	TARGETS lucu
	EXPORT lucuTargets
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
	INCLUDES DESTINATION include
	FILE_SET HEADERS DESTINATION include
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
	lucuConfigVersion.cmake
	VERSION ${PACKAGE_VERSION}
	COMPATIBILITY SameMajorVersion
)

install(
	EXPORT lucuTargets
	FILE lucuConfig.cmake
	NAMESPACE lucu::
	DESTINATION lib/cmake/lucu
)
