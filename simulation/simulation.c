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
// Default values if user does not pass arguments
#define DEFAULT_PICKING_ITEM_AMOUNT 5
#define DEFAULT_RUNS 5
#define DEFAULT_HEURISTIC_WEIGHT 5.0
#define DEFAULT_SEED 123456789
/*
 *#######################################################################
 *##                                                                   ##
 *##                         Simulation functions                      ##
 *##                                                                   ##
 *#######################################################################
 */
int main(int argc, char** argv) {
    //===============================
    // Parse CLI arguments
    //===============================
    int picking_item_amount = DEFAULT_PICKING_ITEM_AMOUNT;
    int runs = DEFAULT_RUNS;
    double heuristic_weight = DEFAULT_HEURISTIC_WEIGHT;
    int seed = DEFAULT_SEED;

    if (argc >= 2) picking_item_amount = atoi(argv[1]);
    if (argc >= 3) runs = atoi(argv[2]);
    if (argc >= 4) heuristic_weight = atof(argv[3]);
    if (argc >= 5) seed = atoi(argv[4]);

    srand(seed);

    //-------------------------------
    // Prepare results.txt
    //-------------------------------
    FILE* results = fopen("results.txt", "w");
    if (!results) {
        fprintf(stderr, "ERROR: Failed to open results.txt\n");
        return EXIT_FAILURE;
    }

    //-------------------------------
    // Create warehouse
    //-------------------------------
    warehouse_t* warehouse = create_warehouse();

    //-------------------------------
    // Start time / clock
    //-------------------------------
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);


    item_t picking_list[AMOUNT_OF_PICKING_ITEMS];
    generate_picking_list(picking_list, warehouse, AMOUNT_OF_PICKING_ITEMS);

    //-------------------------------
    // Creating a robot
    //-------------------------------
    robot_t* robot1 = create_robot(warehouse);

    // Move the robot to a specified point
    move_robot_to_point(robot1, warehouse, 9, 0);
    // The robot finds the items in the picking list, then gets them and returns them to a point.
    robot_get_picking_list(robot1, warehouse, picking_list);

    //-------------------------------
    // Cleanup Heap Memory
    //-------------------------------
    destroy_warehouse(warehouse);
    free_robot(robot1);

    //-------------------------------
    // End time / clock
    //-------------------------------
    clock_gettime(CLOCK_MONOTONIC, &end);
    double runtime = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    //-------------------------------
    // Write machine-readable output
    //-------------------------------
    fprintf(results, "runs=%d\n", runs);
    fprintf(results, "seed=%d\n", seed);
    fprintf(results, "picking_items=%d\n", picking_item_amount);
    fprintf(results, "warehouse=%dx%d\n", rows, columns);
    fprintf(results, "runtime=%.9f\n", runtime);
    fprintf(results, "avg_runtime_per_run=%.9f\n", runtime / runs);
    fprintf(results, "heuristic_weight=%f\n", heuristic_weight);

    fprintf(results, "\n"); // POSIX-friendly file ending
    fclose(results);

    return 0;
}