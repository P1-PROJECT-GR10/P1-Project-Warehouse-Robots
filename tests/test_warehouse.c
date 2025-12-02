#include "warehouse.h"
#include "mtest.h"

#include <stdio.h>
#include <stdlib.h>

/* ------------------ Test 1: Top row empty ------------------ */
TEST_CASE(top_row_empty,
    warehouse_t* warehouse = create_warehouse();

    for (int col = 0; col < warehouse->columns; col++) {
        CHECK_TRUE(warehouse->map[col] == empty);
    }

    destroy_warehouse(warehouse);
)

/* ------------------ Test 2: Bottom row empty ------------------ */
TEST_CASE(bottom_row_empty,
    warehouse_t* warehouse = create_warehouse();

    for (int col = 0; col < warehouse->columns; col++) {
        CHECK_TRUE(warehouse->map[col] == empty);
    }

    destroy_warehouse(warehouse);
)

/* ------------------ Test 3: Shelf placement correctness ------------------ */
TEST_CASE(shelf_row_correctness,
    warehouse_t* warehouse = create_warehouse();

    for (int col = 0; col < warehouse->columns; col++) { // Loop through all columns in this row
    int left_main_aisle   = (col < MAIN_AISLE_WIDTH);                      // Left main aisle columns
    int right_main_aisle  = (col >= MAIN_AISLE_WIDTH*2 + SHELF_LENGTH*2);  // Right main aisle columns
    int center_main_aisle = (col >= MAIN_AISLE_WIDTH + SHELF_LENGTH &&     // Center main aisle columns
                             col <  MAIN_AISLE_WIDTH + SHELF_LENGTH + MAIN_AISLE_WIDTH);
    for (int col = 0; col < warehouse->columns; col++) {
        CHECK_TRUE(warehouse->map[col] == empty);
    }

    if (left_main_aisle || right_main_aisle || center_main_aisle)
        CHECK_TRUE(warehouse->map[AISLE_WIDTH * warehouse->columns + col] == empty); // Main aisles must be empty
    else
        CHECK_TRUE(warehouse->map[AISLE_WIDTH * warehouse->columns + col] == shelf); // All other columns in shelf row must be shelves
}

    destroy_warehouse(warehouse);
)

MAIN_RUN_TESTS(top_row_empty, bottom_row_empty, shelf_row_correctness);