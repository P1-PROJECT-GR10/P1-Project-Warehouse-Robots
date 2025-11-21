add_library(warehouse warehouse.c
        Robot.c
        robot.h)

add_executable(P1_Project main.c
        Robot.c
        robot.h)

target_link_libraries(P1_Project warehouse)