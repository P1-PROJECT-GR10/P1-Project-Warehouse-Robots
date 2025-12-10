add_library(warehouse warehouse.c)
add_library(robot robot.c warehouse.c)
add_library(a_star a_star.c)
add_library(greedy_step greedy_step.c)

target_include_directories(warehouse PUBLIC .)
target_include_directories(robot PUBLIC .)
target_include_directories(a_star PUBLIC .)
target_include_directories(greedy_step PUBLIC .)

add_executable(P1_Project main.c)

target_link_libraries(P1_Project warehouse robot a_star greedy_step)
