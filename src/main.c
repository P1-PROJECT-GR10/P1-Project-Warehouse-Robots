#include <time.h>
#include "generate_picking_list.h"
#include "warehouse.h"
#include "robot.h"
#include "a_star.h"

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

    // Create and display picking list
    int amount_of_picking_items = 5;
    item_t picking_list[amount_of_picking_items];
    generate_picking_list(picking_list, items, amount_of_picking_items, seed, n_shelves);
    display_picking_list(picking_list, amount_of_picking_items);

    // Create robot
    robot_t* robot1 = create_robot();
    warehouse[columns * robot1->y + robot1->x] = robot; //Sets the robot in the warehouse

    // Print the warehouse to the console
    print_warehouse(warehouse, rows, columns);

    // The robot finds the items in the picking list, then gets them and returns them to a point.
    robot_get_picking_list(robot1, warehouse, rows, columns, picking_list, amount_of_picking_items, shelves, n_shelves);

    // Free allocated memory
    for (int i = 0; i < n_shelves; i++) {
        free(shelves[i]);
    }
    free(warehouse);
    free(robot1);

    // Lastly we display the runtime to see how fast it took the robot(s) to complete their assigned tasks
    clock_gettime(CLOCK_MONOTONIC, &end);
    double runtime = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    printf("Runtime: %.9f seconds\n", runtime);
    printf("Seed: %d\n", seed);

    return 0;
}