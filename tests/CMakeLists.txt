add_executable(vector vector.c)

target_include_directories(vector PRIVATE ../include ${CRITERION_INCLUDE_DIRS})
target_link_libraries(vector PRIVATE lucu ${CRITERION_LIBRARIES})

add_test(NAME LucuVector COMMAND ./vector)
