add_library(warehouse warehouse.c
        Robot.c
        robot.h
        bruteforce.c
        bruteforce.h)

add_executable(P1_Project main.c
        bruteforce.c
        bruteforce.h)

target_link_libraries(P1_Project warehouse)
