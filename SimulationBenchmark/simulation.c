#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "warehouse.h"
#include "robot.h"
#include "a_star.h"

// Simulation variables
const int main_aisle_width = MAIN_AISLE_WIDTH;
const int aisle_width      = AISLE_WIDTH;
const int shelf_length     = SHELF_LENGTH;
const int shelves_amount   = SHELF_AMOUNT;
const int n_shelves = SHELF_AMOUNT * SHELF_LENGTH * 2 * 2;
const int rows = shelves_amount * (2 + aisle_width) + aisle_width;
const int columns = main_aisle_width * 3 + shelf_length * 2;

int main(void) {
    // Time Parameter
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    int seed = 123456789;
    srand(seed);


    /* Some simulation outputs
     *A* runtime
     *Path cost
     *Number of expansions
     *Memory usage
     *Runtime across different seeds
     *Runtime across different warehouse sizes
     *Runtime with different heuristic weights
    */


    clock_gettime(CLOCK_MONOTONIC, &end);
    double runtime = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    // Output as machine-readable lines
    printf("runs=%d\n", runs);
    printf("seed=%d\n", seed);
    printf("avg_time=%f\n", runtime / runs);
    printf("avg_path_cost=%ld\n", total_path_cost / runs);
    printf("heuristic_weight=%f\n", heuristic_weight);
    return 0;
}