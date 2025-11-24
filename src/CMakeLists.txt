add_library(warehouse warehouse.c robot.c)
add_library(robot robot.c)
add_library(a_star a_star.c)

target_include_directories(warehouse PUBLIC .)
target_include_directories(robot PUBLIC .)
target_include_directories(a_star PUBLIC .)

add_executable(P1_Project main.c)

target_link_libraries(P1_Project warehouse robot a_star)
