add_executable(Simulation simulation.c)
target_link_libraries(Simulation greedy_step a_star robot)

set_target_properties(Simulation PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)
# add a custom command to run after building the Simulation executable
add_custom_command(TARGET Simulation POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/items.txt          # <-- root of the repo
        $<TARGET_FILE_DIR:Simulation>
)

