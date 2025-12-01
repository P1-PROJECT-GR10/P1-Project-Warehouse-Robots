#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "warehouse.h"
#include "generate_picking_list.h"
#include "robot.h"
#include "a_star.h"
/*
 *#######################################################################
 *##                                                                   ##
 *##                         Simulation variables                      ##
 *##                                                                   ##
 *#######################################################################
 */
// Warehouse variables -> fixed size currently
const int main_aisle_width = MAIN_AISLE_WIDTH;
const int aisle_width      = AISLE_WIDTH;
const int shelf_length     = SHELF_LENGTH;
const int shelves_amount   = SHELF_AMOUNT;
const int n_shelves = SHELF_AMOUNT * SHELF_LENGTH * 2 * 2;
const int rows = shelves_amount * (2 + aisle_width) + aisle_width;
const int columns = main_aisle_width * 3 + shelf_length * 2;
// Cost-functions & time influential variables
const int picking_item_amount = 5;
const int runs = 5;
const double heuristic_weight = 5;
const double total_path_cost = 20;

/*
 *#######################################################################
 *##                                                                   ##
 *##                         Simulation functions                      ##
 *##                                                                   ##
 *#######################################################################
 */
int main(void) {
    // Time/clock
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    int seed = 123456789;
    srand(seed);

    // Seems redundant for a simulation -> filepath has to be selected here
    FILE* items_file = fopen("items.txt", "r");
    if (items_file == NULL) {
        printf("Failed to open file.\n");
        exit(EXIT_FAILURE);
    }
    item_t items[n_shelves];

    // Variable calculations
    long avg_path_cost = total_path_cost / runs;

    // Running simulation
    file_read_items(items, n_shelves, items_file);
    fclose(items_file);

    shelf_t* shelves[n_shelves];
    int* warehouse = generate_layout(main_aisle_width, aisle_width, shelf_length, rows, columns, shelves, items);

    item_t pickingItems[picking_item_amount];
    generate_picking_list(pickingItems, items, picking_item_amount, seed, n_shelves);

    robot_t* robot1 = create_robot();
    warehouse[columns * robot1->y + robot1->x] = robot;

    for (int i = 0; i < n_shelves; i++) {
        free(shelves[i]);
    }

    move_robot_to_point(robot1, warehouse, rows, columns, 14, 0);

    free(warehouse);
    free(robot1);

    // Ends the clock and gets the time.
    clock_gettime(CLOCK_MONOTONIC, &end);
    double runtime = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    // Output as machine-readable lines
    printf("runs=%d\n", runs);
    printf("seed=%d\n", seed);
    printf("warehouse_size=%dx%d\n", rows, columns);
    printf("total_runtime=%.9f seconds\n", runtime);
    printf("avg_time=%f seconds\n", runtime / runs);
    printf("avg_path_cost=%ld\n", avg_path_cost);
    printf("heuristic_weight=%f\n", heuristic_weight);
    return 0;
}