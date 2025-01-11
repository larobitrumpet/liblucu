include_directories(../include/)

file(GLOB SRC_FILES *.c)

add_library(lucu STATIC ${SRC_FILES})
