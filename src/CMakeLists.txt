include_directories(../include/)

file(GLOB SRC_FILES *.c)

add_library(liblucu STATIC ${SRC_FILES})
