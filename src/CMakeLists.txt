add_library(warehouse warehouse.c
        Robot.c)

add_executable(P1_Project main.c
        Robot.c)

target_link_libraries(P1_Project warehouse)