
add_executable(test_warehouse test_warehouse.c)
target_link_libraries(test_warehouse mtest warehouse)

add_executable(test_robot test_robot.c)
target_link_libraries(test_robot mtest warehouse robot a_star)


# Copy items.txt into the test's build directory
configure_file(
        ${PROJECT_SOURCE_DIR}/items.txt
        ${CMAKE_CURRENT_BINARY_DIR}/items.txt
        COPYONLY
)

discover_tests(test_warehouse test_robot)