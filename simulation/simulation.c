#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "warehouse.h"
#include "robot.h"
#include "a_star.h"
#include "greedy_step.h"
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
    bool block_center_aisle;
} warehouse_config_t;

// Default values if user does !pass arguments (This is a 10x18 warehouse layout)
warehouse_config_t config = {
    .shelf_amount = 3,
    .shelf_length = 6,
    .aisle_width = 1,
    .main_aisle_width = 2,
    .block_center_aisle = 0
};
/*
 *#######################################################################
 *##                                                                   ##
 *##                         Simulation functions                      ##
 *##                                                                   ##
 *#######################################################################
 */
warehouse_t* create_simulated_warehouse(warehouse_config_t cfg);
int validate_int(const char* arg, int min, const char* name);
int validate_boolean(const char* arg, int min, int max, const char* name);
cell_e* generate_simulated_layout(const warehouse_t* warehouse, warehouse_config_t cfg);
bool is_vertical_end_aisle_simulated(const warehouse_t* warehouse, int row, warehouse_config_t cfg);
bool is_main_aisle_simulated(int column, warehouse_config_t cfg);

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
    if (argc >= 9) config.block_center_aisle = validate_boolean(argv[8], 0, 1, "block center aisle");

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
    double total_runtime_astar = 0.0;
    double total_runtime_greedy_step = 0.0;
    int A_star_total_steps = 0;
    int bf_total_steps = 0;

    //-------------------------------
    // A* Simulation
    //-------------------------------
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
        picking_list_t* picking_list = generate_picking_list(warehouse, picking_item_amount);
        display_picking_list(picking_list, picking_item_amount);

        // Create robot
        robot_t* robot1 = create_robot(warehouse);

        // Start timer
        struct timespec start, end;
        clock_gettime(CLOCK_MONOTONIC, &start);

        // Run simulation
        robot_get_picking_list(robot1, warehouse, picking_list);
        int A_star_steps = robot1->steps;
        A_star_total_steps += A_star_steps;

        // End timer
        clock_gettime(CLOCK_MONOTONIC, &end);
        double runtime = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
        total_runtime_astar += runtime;

        // Cleanup heap memory
        destroy_warehouse(warehouse);
        free_robot(robot1);

        // Per-run runtime -> only for terminal
        printf("astar_run_%d_runtime=%.9f\n", i + 1, runtime);
        printf("Seed: %d\n",seed+i);
    }

    // safely reset seed to start seed for greedy step simulation
    if (argc >= 4) seed = atoi(argv[3]);

    //-------------------------------
    // greedy step simulation
    //-------------------------------
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
        picking_list_t* picking_list = generate_picking_list(warehouse, picking_item_amount);
        display_picking_list(picking_list, picking_item_amount);

        // Create robot
        robot_t* robot1 = create_robot(warehouse);

        // Start timer
        struct timespec start, end;
        clock_gettime(CLOCK_MONOTONIC, &start);

        // Run simulation
        greedy_step_get_picking_list(robot1, warehouse, picking_list);
        int bf_steps = robot1->steps;
        bf_total_steps += bf_steps;

        // End timer
        clock_gettime(CLOCK_MONOTONIC, &end);
        double runtime = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
        total_runtime_greedy_step += runtime;

        // Cleanup heap memory
        destroy_warehouse(warehouse);
        free_robot(robot1);

        // Per-run runtime -> only for terminal
        printf("greedy_step_run_%d_runtime=%.9f\n", i + 1, runtime);
        printf("current_seed=%d\n", seed+i);
    }

    // Average step calculations -> casting to float for results
    float A_start_average_steps = (float)A_star_total_steps / (float)runs;
    float bf_average_steps = (float)bf_total_steps / (float)runs;

    //---------------------------------------------
    // Write summary of simulations
    //---------------------------------------------
    fprintf(results, "\n=== SUMMARY OF SIMULATIONS ===\n");

    fprintf(results, "runs=%d\n", runs);
    fprintf(results, "start_seed=%d\n", seed);
    fprintf(results, "picking_items=%d\n", picking_item_amount);

    // Warehouse dimensions
    fprintf(results, "warehouse_rows=%d\n",
            config.shelf_amount * (2 + config.aisle_width) + config.aisle_width);

    fprintf(results, "warehouse_columns=%d\n",
            config.main_aisle_width * 3 + config.shelf_length * 2);

    // Separate results
    fprintf(results, "\n=== A_STAR ===\n");
    fprintf(results, "total_runtime_astar=%.9f\n", total_runtime_astar);
    fprintf(results, "avg_runtime_astar=%.9f\n", total_runtime_astar / runs);
    fprintf(results, "total_steps_astar=%d\n", A_star_total_steps);
    fprintf(results, "avg_steps_astar=%.2f\n", A_start_average_steps);

    fprintf(results, "\n=== GREEDY STEP ===\n");
    fprintf(results, "total_runtime_greedy_step=%.9f\n", total_runtime_greedy_step);
    fprintf(results, "avg_runtime_greedy_step=%.9f\n", total_runtime_greedy_step / runs);
    fprintf(results, "total_steps_greedy_step=%d\n", bf_total_steps);
    fprintf(results, "avg_steps_greedy_step=%.2f\n", bf_average_steps);

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

int validate_boolean(const char* arg, int min, int max, const char* name) {
    int value = atoi(arg);
    if (value < min) {
        fprintf(stderr, "ERROR: %s must be >= %d (got %d)\n", name, min, value);
        exit(EXIT_FAILURE);
    }
    if (value > max) {
        fprintf(stderr, "ERROR: %s must be <= %d (got %d)\n", name, max, value);
        exit(EXIT_FAILURE);
    }

    if (value < max)
        return false;

    return true;
}

bool is_vertical_end_aisle_simulated(const warehouse_t* warehouse, int row, warehouse_config_t cfg) {
    if (row < cfg.aisle_width
        || row >= warehouse->rows - cfg.aisle_width){
        return true; // Top & bottom aisles
        }
    return false;
}

bool is_main_aisle_simulated(int column, warehouse_config_t cfg) {
    if (column >= cfg.main_aisle_width+cfg.shelf_length
        && column < cfg.main_aisle_width + cfg.shelf_length + cfg.main_aisle_width){
        return true; // Center main aisle
        }

    if (column < cfg.main_aisle_width
        || column >= cfg.main_aisle_width * 2 + cfg.shelf_length * 2){
        return true; // Left & right side main aisles
        }
    return false;
}

cell_e* generate_simulated_layout(const warehouse_t* warehouse, warehouse_config_t cfg) {
    const int rows = warehouse->rows;
    const int columns = warehouse->columns;
    const int shelf_width = 2;

    cell_e* map = (cell_e*)safe_malloc(sizeof(cell_e)*columns*rows); // Allocate memory for the map

    int row_pattern = 0;
    for (int row = 0; row < rows; row++) {
        bool is_end_aisle = is_vertical_end_aisle_simulated(warehouse, row, cfg); // Bool per row: Is aisle is at the ends (top or bottom)

        if (!is_end_aisle) {
            row_pattern++; // Pattern inside the rows with shelves & aisles inbetween shelves
        }

        for (int col = 0; col < columns; col++) {
            int id = row * columns + col; // Array position

            if (is_end_aisle) {
                map[id] = empty; // Top & bottom aisles
                continue;
            }

            if (is_main_aisle_simulated(col, cfg)) {
                map[id] = empty; // Left, right & center main aisles
                continue;
            }

            if (row_pattern > cfg.aisle_width + shelf_width) {
                row_pattern = 1; // Restart pattern
            }

            if (row_pattern <= shelf_width) {
                map[id] = shelf; // Pattern is within shelf width; must be shelf
            } else {
                map[id] = empty; // Outside of shelf width; must be aisle
            }
        }
    }
    return map;
}

// Reconfigured create_warehouse() function to take CLI parsed inputs for size
warehouse_t* create_simulated_warehouse(warehouse_config_t cfg) {

    warehouse_t* warehouse = safe_malloc(sizeof(*warehouse));

    warehouse->rows    = cfg.shelf_amount * (2 + cfg.aisle_width) + cfg.aisle_width; // Sum = rows
    warehouse->columns = cfg.main_aisle_width * 3 + cfg.shelf_length * 2; // Sum = columns
    warehouse->number_of_shelves = cfg.shelf_amount * cfg.shelf_length * 2 * 2; // Sum = n_shelves
    // Function based struct variables.
    warehouse->drop_zones = generate_drop_zones(AMOUNT_OF_DROP_ZONES); // fixed drop_zone amount for now
    warehouse->map = generate_simulated_layout(warehouse, cfg);
    warehouse->items = read_items_from_file(ITEM_FILE, &warehouse->number_of_items);
    warehouse->shelves = populate_shelves(warehouse);
    warehouse->printing = false;

    if (cfg.block_center_aisle) {
        block_center_aisle(warehouse);
    }

    set_drop_zone_cell(warehouse, warehouse->columns-1, warehouse->rows/2-1);
    set_drop_zone_cell(warehouse, warehouse->columns-1, warehouse->rows/2);

    return warehouse;
}