add_library(warehouse warehouse.c)
add_library(generate_picking_list generate_picking_list.c)
add_library(robot robot.c warehouse.c)
add_library(a_star a_star.c)

target_include_directories(warehouse PUBLIC .)
target_include_directories(generate_picking_list PUBLIC .)
target_include_directories(robot PUBLIC .)
target_include_directories(a_star PUBLIC .)

add_executable(P1_Project main.c)

target_link_libraries(P1_Project warehouse generate_picking_list robot a_star)
