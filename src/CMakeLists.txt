add_library(warehouse warehouse.c)
add_library(generate_picking_list generate_picking_list.c)
add_library(robot robot.c)

add_executable(P1_Project main.c)

target_link_libraries(P1_Project warehouse generate_picking_list robot)
