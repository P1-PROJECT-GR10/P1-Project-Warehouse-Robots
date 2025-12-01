
add_executable(Simulation simulation.c)
target_link_libraries(Simulation a_star robot generate_picking_list)

set_target_properties(Simulation PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)
