#include <time.h>
#include "generate_picking_list.h"
#include "warehouse.h"
#include "robot.h"
#include "a_star.h"
#include "bruteforce.h"

int main(void) {
    // Time Parameters and functionality
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    // Seed pseudo-random number generator, with a set seed
    int seed = 123456789;
    srand(seed);

    // Create warehouse:
    warehouse_t* warehouse = create_warehouse();

    // Set drop zones
    set_drop_zone_cell(warehouse, 17, 4);
    set_drop_zone_cell(warehouse, 17, 5);

    // Generate picking list
    item_t picking_list[AMOUNT_OF_PICKING_ITEMS];
    generate_picking_list(picking_list, warehouse, AMOUNT_OF_PICKING_ITEMS);
    display_picking_list(picking_list, AMOUNT_OF_PICKING_ITEMS);

    // Create a robot
    robot_t* robot1 = create_robot(warehouse);
    print_warehouse(warehouse);

    // Move the robot to a specified point
    move_robot_to_point(robot1, warehouse, 9, 0);
    // The robot finds the items in the picking list, then gets them and returns them to a point.
    robot_get_picking_list(robot1, warehouse, picking_list);

    // Free allocated memory
    free_robot(robot1);
    destroy_warehouse(warehouse);

    // Lastly we display the runtime to see how fast it took the robot(s) to complete their assigned tasks
    clock_gettime(CLOCK_MONOTONIC, &end);
    const double runtime = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    printf("Runtime: %.9f seconds\n", runtime);
    printf("Seed: %d\n", seed);

    return 0;
}
