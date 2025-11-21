
add_executable(test_warehouse test_warehouse.c ../src/warehouse.c)
target_link_libraries(test_warehouse)

add_test(NAME r_warehouse_test COMMAND test_warehouse)