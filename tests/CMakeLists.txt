add_executable(vector vector.c)
add_executable(option option.c)

target_include_directories(vector PRIVATE ../include ${CRITERION_INCLUDE_DIRS})
target_include_directories(option PRIVATE ../include ${CRITERION_INCLUDE_DIRS})

target_link_libraries(vector PRIVATE lucu ${CRITERION_LIBRARIES})
target_link_libraries(option PRIVATE lucu ${CRITERION_LIBRARIES})

add_test(NAME LucuVector COMMAND ./vector)
add_test(NAME LucuOption COMMAND ./option)
