add_executable(vector vector.c)
add_executable(heap heap.c)
add_executable(cache cache.c)

target_include_directories(vector PRIVATE ../include ${CRITERION_INCLUDE_DIRS})
target_include_directories(heap PRIVATE ../include ${CRITERION_INCLUDE_DIRS})
target_include_directories(cache PRIVATE ../include ${CRITERION_INCLUDE_DIRS})

target_link_libraries(vector PRIVATE lucu ${CRITERION_LIBRARIES})
target_link_libraries(heap PRIVATE lucu ${CRITERION_LIBRARIES})
target_link_libraries(cache PRIVATE lucu ${CRITERION_LIBRARIES})

add_test(NAME LucuVector COMMAND ./vector)
add_test(NAME LucuHeap COMMAND ./heap)
add_test(NAME LucuCache COMMAND ./cache)
