#include "../src/warehouse.h"
#include "mtest.h"

#include <stdio.h>
#include <stdlib.h>

/* Arrange */
const int main_aisle_width = MAIN_AISLE_WIDTH;
const int aisle_width      = AISLE_WIDTH;
const int shelf_length     = SHELF_LENGTH;
const int shelves_amount   = SHELF_AMOUNT;
const int n_shelves = SHELF_AMOUNT * SHELF_LENGTH * 2 * 2;
const int rows = shelves_amount * (2 + aisle_width) + aisle_width;
const int columns = main_aisle_width * 3 + shelf_length * 2;

/* ------------------ Test 1: Top row empty ------------------ */
TEST_CASE(top_row_empty,

    FILE* items_file = fopen("test-items.txt", "r");
    if (items_file == NULL) {
        printf("Failed to open file.\n");
        exit(EXIT_FAILURE);
    }
    item_t items[n_shelves];
    shelf_t* shelves[n_shelves];

    int *warehouse = generate_layout(
        main_aisle_width,
        aisle_width,
        shelf_length,
        rows,
        columns,
        shelves,
        items
    );

    // int item_count = file_read_items(items, n_shelves, items_file);
    fclose(items_file);

    // CHECK_FAIL(warehouse != NULL);

    for (int col = 0; col < columns; col++) {
        CHECK_TRUE(warehouse[col] == empty);
    }
    free(warehouse);
    )

/* ------------------ Test 2: Bottom row empty --------------- */
TEST_CASE(bottom_row_empty,
    FILE* items_file = fopen("test-items.txt", "r");
    if (items_file == NULL) {
        printf("Failed to open file.\n");
        exit(EXIT_FAILURE);
    }
    item_t items[n_shelves];
    shelf_t* shelves[n_shelves];

    int *warehouse = generate_layout(
        main_aisle_width,
        aisle_width,
        shelf_length,
        rows,
        columns,
        shelves,
        items
    );

    // int item_count = file_read_items(items, n_shelves, items_file);
    fclose(items_file);

    // CHECK_FAIL(warehouse != NULL);

    int bottom = (rows - 1) * columns;
    for (int col = 0; col < columns; col++) {
        CHECK_TRUE(warehouse[bottom + col] == empty);
    }
    free(warehouse);
    )

/* ------------------ Test 3: Shelf row correctness ---------- */
TEST_CASE(shelf_row_correctness,
    FILE* items_file = fopen("test-items.txt", "r");
        if (items_file == NULL) {
            printf("Failed to open file.\n");
            exit(EXIT_FAILURE);
        }
        item_t items[n_shelves];
        shelf_t* shelves[n_shelves];

    int *warehouse = generate_layout(
        main_aisle_width,
        aisle_width,
        shelf_length,
        rows,
        columns,
        shelves,
        items
    );

    // int item_count = file_read_items(items, n_shelves, items_file);
    fclose(items_file);

    // CHECK_FAIL(warehouse != NULL);
/*
    First shelf row is ALWAYS row index = aisle_width
    because row 0 = aisle, row 1 = aisle_width=1
*/
int shelf_row = aisle_width;               // First shelf row index (just below the top aisle)
int base = shelf_row * columns;            // Starting index of this row in the 1D warehouse array

for (int col = 0; col < columns; col++) { // Loop through all columns in this row
    int left_main_aisle   = (col < main_aisle_width);                      // Left main aisle columns
    int right_main_aisle  = (col >= main_aisle_width*2 + shelf_length*2);  // Right main aisle columns
    int center_main_aisle = (col >= main_aisle_width + shelf_length &&     // Center main aisle columns
                             col <  main_aisle_width + shelf_length + main_aisle_width);

    if (left_main_aisle || right_main_aisle || center_main_aisle)
        CHECK_TRUE(warehouse[base + col] == empty); // Main aisles must be empty
    else
        CHECK_TRUE(warehouse[base + col] == shelf); // All other columns in shelf row must be shelves
}
free(warehouse);
    )

MAIN_RUN_TESTS(top_row_empty, bottom_row_empty, shelf_row_correctness);