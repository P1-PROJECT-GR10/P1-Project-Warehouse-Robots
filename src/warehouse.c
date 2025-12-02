#include "warehouse.h"
#include <stdio.h>
#include <stdlib.h>

item_t* read_items_from_file(char* file_name) {

    int n_shelves = SHELF_AMOUNT * SHELF_LENGTH * 2 * 2;

    FILE* items_file = fopen(file_name, "r");
    if (items_file == NULL) {
        printf("Failed to open file.\n");
        printf("Remember to set working directory.\n");
        exit(EXIT_FAILURE);
    }

    item_t* items = malloc(sizeof(item_t)*n_shelves);
    file_read_items(items, n_shelves, items_file);
    fclose(items_file);

    return items;
}

warehouse_t* create_warehouse() {

    warehouse_t* warehouse = (warehouse_t*)malloc(sizeof(warehouse_t));

    warehouse->rows = SHELF_AMOUNT * (2 + AISLE_WIDTH) + AISLE_WIDTH;
    warehouse->columns = MAIN_AISLE_WIDTH * 3 + SHELF_LENGTH * 2;
    warehouse->number_of_shelves = SHELF_AMOUNT * SHELF_LENGTH * 2 * 2;
    warehouse->drop_zones = generate_drop_zones(AMOUNT_OF_DROP_ZONES);
    warehouse->shelves = malloc(sizeof(shelf_t*)*warehouse->number_of_shelves);
    warehouse->items = read_items_from_file(ITEM_FILE);
    warehouse->map = generate_layout(warehouse);

    return warehouse;
}

void destroy_warehouse(warehouse_t* warehouse) {

    free(warehouse->items);
    free(warehouse->map);
    free_shelves(warehouse->shelves, warehouse->number_of_shelves);
    free(warehouse->drop_zones);
    free(warehouse);
}

cell_e* generate_layout(const warehouse_t* warehouse) {

    const int rows = warehouse->rows;
    const int columns = warehouse->columns;

    cell_e* map = (cell_e*)malloc(sizeof(cell_e)*columns*rows);

    int shelf_count = 0;
    for (int i = 0, l = 0; i < rows; i++) {
        // i: Row count
        // l: Working row count
        if (i > AISLE_WIDTH-1 && i < rows-AISLE_WIDTH) { // If i is inbetween top & bottom aisle boundary, l++
            l++;
        }
        for (int j = i * columns, k = 0; j < i * columns + columns; j++, k++) {
            // j: True array position
            // k: Working column count
            if (i <= AISLE_WIDTH-1 || i >= rows-AISLE_WIDTH) { // Top & bottom aisle boundary
                map[j] = empty; // Top & bottom aisle
            } else { // Generate the rows inbetween top & bottom aisles
                if (k < MAIN_AISLE_WIDTH || k >= MAIN_AISLE_WIDTH*2+SHELF_LENGTH*2 || // Side main aisle boundaries
                    k >= MAIN_AISLE_WIDTH+SHELF_LENGTH && k < MAIN_AISLE_WIDTH+SHELF_LENGTH+MAIN_AISLE_WIDTH) { // Main aisle boundary
                    map[j] = empty; // Main aisle
                } else {
                    if (l > AISLE_WIDTH + 2) {l = 1;} // If l > aisle_width + shelf_width, reset working column count
                    if (l <= 2) { // If l <= shelf_width, place shelf
                        map[j]= shelf;
                        warehouse->shelves[shelf_count] = generate_shelf(warehouse->items[shelf_count], 10, k, i);
                        //printf("[%d] %s %s\n",shelf_count, shelves[shelf_count]->item.color, shelves[shelf_count]->item.name);
                        shelf_count++;
                    } else { // Else row must be aisle
                        map[j] = empty;
                    }
                }
            }
        }
    }
    return map;
}

drop_zones* generate_drop_zones(int capacity) {
    drop_zones* zones = (drop_zones*)malloc(sizeof(drop_zones));
    zones->amount = 0;
    zones->capacity = capacity;
    zones->zones = (drop_zone_t**)malloc(sizeof(drop_zone_t*) * capacity);

    return zones;
}

void set_drop_zone_cell(warehouse_t* warehouse, const int x, const int y) {
    cell_e* cell = get_cell(warehouse, x, y); // Remove magic number, once columns is made global definition

    if (warehouse->drop_zones->amount >= warehouse->drop_zones->capacity) {
        printf("Maximum amount of drop zones already reached\n");
        return;
    }

    if (*cell != shelf && *cell != drop_zone) {
        *cell = drop_zone;
        drop_zone_t drop_zone;
        drop_zone.x = x;
        drop_zone.y = y;
        warehouse->drop_zones->zones[warehouse->drop_zones->amount] = &drop_zone;
        warehouse->drop_zones->amount++;
    }
}

void print_cell(cell_e cell) {
    switch (cell) {
        case empty:
            printf("| ");
            break;
        case shelf:
            printf("|X");
            break;
        case robot:
            printf("|R");
    }

}

cell_e* get_cell(const warehouse_t* warehouse, int x, int y) {
    return &warehouse->map[y * warehouse->columns + x];
}

void print_warehouse(const warehouse_t* warehouse) {

    int rows = warehouse->rows;
    int columns = warehouse->columns;

    // Print row of x-coords
    printf("Y: X:");
    for (int x = 0; x < columns; x++) {
        printf("%d ", x % 10);
    }
    printf("\n");

    for (int y = 0; y < rows; y++) {
        printf("%d - ", y % 10);    // Prints y-coords
        for (int x = 0; x < columns; x++) {
            cell_e* cell = get_cell(warehouse, x, y);
            print_cell(*cell);
        }
        printf("|\n"); // Prints | at the end of each row and skips to new line
    }
}

void file_read_items(item_t* items, int n_items, FILE* file) {
    item_t item;
    int i;
    for (i = 0; i < n_items; i++) {
        int success = fscanf(file, " %s %s %lf", item.color, item.name, &item.weight);
        if(success != 3){
            printf("Failed to read enough fields for item %d, only read %d field(s).", i, success);
            exit(EXIT_FAILURE);
        }
        items[i] = item;
    }
    if (i < n_items) {
        printf("Failed to generate %d items, only read %d items from file.\n", n_items, i);
    }
}

struct shelf* generate_shelf(item_t item, int stock, int x, int y) {
    shelf_t* shelf = (shelf_t*)malloc(sizeof(shelf_t));
    shelf->item = item;
    shelf->stock = stock;
    shelf->x = x;
    shelf->y = y;
    return shelf;
}

shelf_t* search_item(char search_input_title[20], char search_input_color[20], const warehouse_t* warehouse) {
    int n_shelves = warehouse->number_of_shelves;
    for (int i = 0; i < n_shelves; i++) {
        if (strcmp(warehouse->shelves[i]->item.color, search_input_color) == 0 && // Using string compare to find the shelf where the item is located.
            strcmp(warehouse->shelves[i]->item.name, search_input_title) == 0) {
            return warehouse->shelves[i];
        }
    }
    return NULL;
}

shelf_t* manual_search_item(const warehouse_t* warehouse) {

    int n_shelves = warehouse->number_of_shelves;

    char search_input_color[20];
    char search_input_name[20];

    printf("\nWrite your search input>");
    scanf(" %6s %9s", search_input_color, search_input_name); //input from user, assigns the string values to search_input_color and search_input_name


    for (int i = 0; i < n_shelves; i++) {
        if (strcmp(warehouse->shelves[i]->item.color, search_input_color) == 0 && // String compare to find the correct shelf that holds the searched item.
            strcmp(warehouse->shelves[i]->item.name, search_input_name) == 0) {
            return warehouse->shelves[i];
            }
    }
    return 0;
}

void free_shelves(shelf_t** shelves, const int n_shelves){
    for (int i = 0; i < n_shelves; i++) {
        free(shelves[i]);
    }
}