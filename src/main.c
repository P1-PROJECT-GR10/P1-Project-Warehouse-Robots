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
    int seed = 123456789;
    srand(seed);

    // Create warehouse:
    warehouse_t* warehouse = create_warehouse();

    // TODO: Change these functions to accept new warehouse handle
    //set_drop_zone_cell(warehouse, drop_zones, 4, 17);
    //set_drop_zone_cell(warehouse, drop_zones, 5, 17);

    item_t pickingItems[AMOUNT_OF_PICKING_ITEMS];
    generate_picking_list(pickingItems, warehouse, AMOUNT_OF_PICKING_ITEMS);
    display_picking_list(pickingItems, AMOUNT_OF_PICKING_ITEMS);

    /*
    shelf_t picking_shelves[AMOUNT_OF_PICKING_ITEMS];

    for (int i = 0; i < AMOUNT_OF_PICKING_ITEMS; i++){
        // Sets picking_shelve to the corresponding shelf that contains an item from the picking list
        // TODO: Rewrite function to incorporate warehouse handle
        picking_shelves[i] = *search_item(pickingItems[i].name,pickingItems[i].color, shelves, n_shelves);
    }
    */

    robot_t* robot1 = create_robot();

    // TODO: Move code below into create_robot function
    int robot1_index = get_index(robot1->x, robot1->y, warehouse->columns);
    warehouse->map[robot1_index] = robot; // Sets the robot in the warehouse

    print_warehouse(warehouse);

    // Move the robot to a specified point
    move_robot_to_point(robot1, warehouse, 0, 0);

    // Free allocated memory
    free_robot(robot1);
    destroy_warehouse(warehouse);

    // Lastly we display the runtime to see how fast it took the robot(s) to complete their assigned tasks
    clock_gettime(CLOCK_MONOTONIC, &end);
    double runtime = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    printf("Runtime: %.9f seconds\n", runtime);
    printf("Seed: %d\n", seed);

    return 0;
}
