#include "warehouse.h"

int main(void) {
    const int rows = SHELF_AMOUNT * (2 + AISLE_WIDTH) + AISLE_WIDTH;
    const int columns = (MAIN_AISLE_WIDTH * 3 + SHELF_LENGTH * 2);
    const int n_shelves = SHELF_AMOUNT * SHELF_LENGTH * 2 * 2;

    FILE* items_file = fopen("items.txt", "r");
    if (items_file == NULL) {
        printf("Failed to open file.\n");
        printf("Remember to set working directory.\n");
        exit(EXIT_FAILURE);
    }

    item_t items[n_shelves];

    int item_count = file_read_items(items, n_shelves, items_file);
    fclose(items_file);

    for (int i = 0; i < item_count; i++) {
        item_t item = items[i];
        printf("%s %s | Weight: %lf\n", item.color, item.name, item.weight);
    }

    shelf_t* shelves[n_shelves];

    int* warehouse = generate_layout(MAIN_AISLE_WIDTH, AISLE_WIDTH, SHELF_LENGTH, rows, columns, shelves, items);
    printf("Generated layout");

    for (int i = 0; i < n_shelves; i++) {
        shelf_t shelf_i = *shelves[i];
    }

    for (int i = 0; i < n_shelves; i++) {
        free(shelves[i]);
    }

    print_warehouse(warehouse, rows, columns);

    free(warehouse);

    return 0;
}