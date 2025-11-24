
add_executable(test_warehouse test_warehouse.c ../src/warehouse.c)
target_link_libraries(test_warehouse mtest)

# Copy test data file (test-items.txt) into the test's build directory
configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/test-items.txt
        ${CMAKE_CURRENT_BINARY_DIR}/test-items.txt
        COPYONLY
)

discover_tests(test_warehouse)