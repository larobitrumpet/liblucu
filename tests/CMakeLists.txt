add_executable(vector vector.c)

target_link_libraries(vector -lcriterion ${CMAKE_CURRENT_BINARY_DIR}/../src/liblucu.a)
add_test(NAME LucuVector COMMAND ./vector)
