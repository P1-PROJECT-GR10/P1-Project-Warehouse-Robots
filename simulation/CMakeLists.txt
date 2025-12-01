
add_executable(Simulation simulation.c)
target_link_libraries(Simulation a_star robot generate_picking_list)
