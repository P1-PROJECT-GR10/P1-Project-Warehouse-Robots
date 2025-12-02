#include <time.h>
#include "generate_picking_list.h"
#include "warehouse.h"
#include "robot.h"
#include "a_star.h"
#include "bruteforce.h"

int main(void) {
    const int rows = SHELF_AMOUNT * (2 + AISLE_WIDTH) + AISLE_WIDTH;
    const int columns = (MAIN_AISLE_WIDTH * 3 + SHELF_LENGTH * 2);
    const int n_shelves = SHELF_AMOUNT * SHELF_LENGTH * 2 * 2;

    // Time Parameters and functionality
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    int seed = 123456789;
    srand(seed);

    // Open item file
    FILE* items_file = fopen("items.txt", "r");
    if (items_file == NULL) {
        printf("Failed to open file.\n");
        printf("Remember to set working directory.\n");
        exit(EXIT_FAILURE);
    }

    // Read item file to items array
    item_t items[n_shelves];
    file_read_items(items, n_shelves, items_file);
    fclose(items_file);

    // Create shelves and generate warehouse
    shelf_t* shelves[n_shelves];
    int* warehouse = generate_layout(MAIN_AISLE_WIDTH, AISLE_WIDTH, SHELF_LENGTH, rows, columns, shelves, items);

    // Sets the drop zones in the warehouse.
    drop_zones* drop_zones = generate_drop_zones(AMOUNT_OF_DROP_ZONES);
    set_drop_zone_cell(warehouse, drop_zones, 17, 4);
    set_drop_zone_cell(warehouse, drop_zones, 17, 5);

    // Generate the picking list
    item_t pickingItems[AMOUNT_OF_PICKING_ITEMS];
    generate_picking_list(pickingItems, items, AMOUNT_OF_PICKING_ITEMS, seed, n_shelves);
    display_picking_list(pickingItems, AMOUNT_OF_PICKING_ITEMS);


    shelf_t picking_shelves[AMOUNT_OF_PICKING_ITEMS];
    for (int i = 0; i < AMOUNT_OF_PICKING_ITEMS; i++){
        picking_shelves[i] = *search_item(pickingItems[i].name,pickingItems[i].color, shelves, n_shelves); // Sets picking_shelve to the corresponding shelf that contains an item from the picking list
    }

    // Create a robot
    robot_t* robot1 = create_robot();
    warehouse[columns * robot1->y + robot1->x] = robot; // Sets the robot in the warehouse

    // Print the warehouse to the console
    print_warehouse(warehouse, rows, columns);

    // move the robot to a point
    move_robot_to_point(robot1, warehouse, rows, columns, 9, 9);

    // The robot finds the items in the picking list, then gets them and returns them to a point.
    robot_get_picking_list(robot1, warehouse, rows, columns, pickingItems, AMOUNT_OF_PICKING_ITEMS, shelves, n_shelves);


    free_warehouse(warehouse);
    free_robot(robot1);
    free_shelves(shelves, n_shelves);
    free(drop_zones);

    // Lastly we display the runtime to see how fast it took the robot(s) to complete their assigned tasks
    clock_gettime(CLOCK_MONOTONIC, &end);
    double runtime = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    printf("Runtime: %.9f seconds\n", runtime);
    printf("Seed: %d\n", seed);

    return 0;
}
