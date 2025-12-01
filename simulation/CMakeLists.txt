add_executable(Simulation simulation.c)
target_link_libraries(Simulation a_star robot generate_picking_list)

set_target_properties(Simulation PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)
# add a custom command to run after building the Simulation executable
add_custom_command(TARGET Simulation POST_BUILD
        # Use CMake's built-in command-line tool to execute a helper command
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        # Source file to copy (items.txt in the current source directory)
        ${CMAKE_CURRENT_SOURCE_DIR}/items.txt
        # Destination directory: the folder where the Simulation executable is located
        $<TARGET_FILE_DIR:Simulation>
)

