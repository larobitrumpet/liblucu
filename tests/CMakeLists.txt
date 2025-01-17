add_executable(vector vector.c)

target_link_libraries(vector PRIVATE lucu -lcriterion)

add_test(NAME LucuVector COMMAND ./vector)
