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
// Default values if user does !pass arguments
#define DEFAULT_PICKING_ITEM_AMOUNT 5
#define DEFAULT_RUNS 1
#define DEFAULT_SEED 123456789

typedef struct {
    int shelf_amount;
    int shelf_length;
    int aisle_width;
    int main_aisle_width;
    int drop_zones;
} warehouse_config_t;

// Default values if user does !pass arguments (This is a 10x18 warehouse layout)
warehouse_config_t config = {
    .shelf_amount = 3,
    .shelf_length = 6,
    .aisle_width = 1,
    .main_aisle_width = 2
};
/*
 *#######################################################################
 *##                                                                   ##
 *##                         Simulation function                       ##
 *##                                                                   ##
 *#######################################################################
 */
warehouse_t* create_simulated_warehouse(warehouse_config_t cfg);
int validate_int(const char* arg, int min, const char* name);

int main(int argc, char** argv) {
    //===============================
    // Parse CLI arguments
    //===============================
    int picking_item_amount = DEFAULT_PICKING_ITEM_AMOUNT;
    int runs = DEFAULT_RUNS;
    int seed = DEFAULT_SEED;

    if (argc >= 2) picking_item_amount = atoi(argv[1]);
    if (argc >= 3) runs = atoi(argv[2]);
    if (argc >= 4) seed = atoi(argv[3]);

    //=========== Warehouse config from CLI ===========
    if (argc >= 5) config.shelf_amount = validate_int(argv[4], 1, "shelf amount");
    if (argc >= 6) config.shelf_length = validate_int(argv[5], 1, "shelf length");
    if (argc >= 7) config.aisle_width = validate_int(argv[6], 1, "aisle width");
    if (argc >= 8) config.main_aisle_width = validate_int(argv[7], 1, "main aisle width");

    //=========== Warehouse size error handling ===========
    if (config.shelf_amount * config.shelf_length > 1000) {
        fprintf(stderr, "ERROR: Too many shelves. Choose smaller values.\n");
        exit(EXIT_FAILURE);
    }

    //-------------------------------
    // Prepare results.txt
    //-------------------------------
    FILE* results = fopen("results.txt", "w");
    if (!results) {
        fprintf(stderr, "ERROR: Failed to open results.txt\n");
        return EXIT_FAILURE;
    }

    //-------------------------------
    // Run simulations multiple times
    //-------------------------------
    double total_runtime = 0.0;

    for (int i = 0; i < runs; i++) {
        //-------------------------------
        // Seed random number generator -> random seed per run
        //-------------------------------
        srand(seed+i);
        // Create warehouse
        warehouse_t* warehouse = create_simulated_warehouse(config);
        if (!warehouse) { fprintf(stderr, "ERROR: Failed to create warehouse\n"); exit(EXIT_FAILURE);}
        if (warehouse->rows > 500 || warehouse->columns > 500) {
            fprintf(stderr, "ERROR: Warehouse size exceeds maximum 500x500.\n");
            exit(EXIT_FAILURE);
        }

        // Generate picking list
        item_t picking_list[picking_item_amount];
        generate_picking_list(picking_list, warehouse, picking_item_amount);

        // Create robot
        robot_t* robot1 = create_robot(warehouse);

        // Start timer
        struct timespec start, end;
        clock_gettime(CLOCK_MONOTONIC, &start);

        // Run simulation
        robot_get_picking_list(robot1, warehouse, picking_list);

        // End timer
        clock_gettime(CLOCK_MONOTONIC, &end);
        double runtime = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
        total_runtime += runtime;

        // Cleanup heap memory
        destroy_warehouse(warehouse);
        free_robot(robot1);

        // Per-run run runtime
        fprintf(results, "run_%d_runtime=%.9f\n", i + 1, runtime);
    }

    //-------------------------------
    // Write summary
    //-------------------------------
    fprintf(results, "\nruns=%d\n", runs);
    fprintf(results, "seed=%d\n", seed);
    fprintf(results, "picking_items=%d\n", picking_item_amount);
    fprintf(results, "warehouse=%dx%d\n",
            config.shelf_amount * (2 + config.aisle_width) + config.aisle_width,
            config.main_aisle_width * 3 + config.shelf_length * 2);
    fprintf(results, "total_runtime=%.9f\n", total_runtime);
    fprintf(results, "avg_runtime_per_run=%.9f\n", total_runtime / runs);

    fclose(results);
    return 0;
}

// Validate user_input parameters from CLI parsing
int validate_int(const char* arg, int min, const char* name) {
    int value = atoi(arg);
    if (value < min) {
        fprintf(stderr, "ERROR: %s must be >= %d (got %d)\n", name, min, value);
        exit(EXIT_FAILURE);
    }
    return value;
}

// Reconfigured create_warehouse() function to take CLI parsed inputs for size
warehouse_t* create_simulated_warehouse(warehouse_config_t cfg) {

    warehouse_t* warehouse = safe_malloc(sizeof(*warehouse));

    warehouse->rows    = cfg.shelf_amount * (2 + cfg.aisle_width) + cfg.aisle_width; // Sum = rows
    warehouse->columns = cfg.main_aisle_width * 3 + cfg.shelf_length * 2; // Sum = columns
    warehouse->number_of_shelves = cfg.shelf_amount * cfg.shelf_length * 2 * 2; // Sum = n_shelves
    // Function based struct variables.
    warehouse->drop_zones = generate_drop_zones(AMOUNT_OF_DROP_ZONES); // fixed drop_zone amount for now
    warehouse->items = read_items_from_file(ITEM_FILE);
    warehouse->map = generate_layout(warehouse);
    warehouse->shelves = populate_shelves(warehouse);

    set_drop_zone_cell(warehouse, 17, 4); // fixed position might not with larger layout
    set_drop_zone_cell(warehouse, 17, 5); // fixed position might not with larger layout

    return warehouse;
}