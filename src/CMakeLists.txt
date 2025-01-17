file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/include/lucu/*.c")

add_library(lucu vector.c ${HEADER_LIST})
target_include_directories(lucu PUBLIC ../include)

source_group(
	TREE "${PROJECT_SOURCE_DIR}/include"
	PREFIX "Header Files"
	FILES ${HEADER_LIST}
)
