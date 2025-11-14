add_library(warehouse warehouse.c)

add_executable(P1_Project main.c)

target_link_libraries(P1_Project warehouse)