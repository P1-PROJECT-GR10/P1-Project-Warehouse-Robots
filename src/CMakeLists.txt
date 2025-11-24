add_library(warehouse warehouse.c
        Robot.c
        robot.h)

add_executable(P1_Project main.c)

target_link_libraries(P1_Project warehouse)
