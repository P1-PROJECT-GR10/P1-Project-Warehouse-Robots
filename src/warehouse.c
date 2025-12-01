#include "warehouse.h"
#include <stdio.h>
#include <stdlib.h>

int* generate_layout(const int main_aisle_width, const int aisle_width, const int shelf_length, const int rows, const int columns, shelf_t* shelves[], item_t* items) {
    int* warehouse = (int*)malloc(sizeof(*warehouse)*columns*rows);

    int shelf_count = 0;
    for (int i = 0, l = 0; i < rows; i++) {
        // i: Row count
        // l: Working row count
        if (i > aisle_width-1 && i < rows-aisle_width) { // If i is inbetween top & bottom aisle boundary, l++
            l++;
        }
        for (int j = i * columns, k = 0; j < i * columns + columns; j++, k++) {
            // j: True array position
            // k: Working column count
            if (i <= aisle_width-1 || i >= rows-aisle_width) { // Top & bottom aisle boundary
                warehouse[j] = empty; // Top & bottom aisle
            } else { // Generate the rows inbetween top & bottom aisles
                if (k < main_aisle_width || k >= main_aisle_width*2+shelf_length*2 || // Side main aisle boundaries
                    k >= main_aisle_width+shelf_length && k < main_aisle_width+shelf_length+main_aisle_width) { // Main aisle boundary
                    warehouse[j] = empty; // Main aisle
                } else {
                    if (l > aisle_width + 2) {l = 1;} // If l > aisle_width + shelf_width, reset working column count
                    if (l <= 2) { // If l <= shelf_width, place shelf
                        warehouse[j]= shelf;
                        shelves[shelf_count] = generate_shelf(items[shelf_count], 10, k, i);
                        shelf_count++;
                    } else { // Else row must be aisle
                        warehouse[j] = empty;
                    }
                }
            }
        }
    }
    return warehouse;
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

int* get_cell(int* warehouse, int columns, int x, int y) {
    return &warehouse[y * columns + x];
}

void print_warehouse(int* warehouse, int rows, int columns) {
    // Print row of x-coords
    printf("Y: X:");
    for (int i = 0; i < columns; i++) {
        printf("%d ", i % 10);
    }
    printf("\n");

    for (int i = 0; i < rows; i++) {
        printf("%d - ", i % 10);    // Prints y-coords
        for (int j = 0; j < columns; j++) {
            int* cell = get_cell(warehouse, columns, j, i);
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

shelf_t* search_item(char search_input_color[20], char search_input_title[20], shelf_t* shelves[], int n_shelves) {
    for (int i = 0; i < n_shelves; i++) {
        if (strcmp(shelves[i]->item.color, search_input_color) == 0 &&
            strcmp(shelves[i]->item.name, search_input_title) == 0) {
            return shelves[i];
        }
    }
    return 0;
}

shelf_t* manual_search_item(shelf_t* shelves[], int n_shelves) {

    char search_input_color[20];
    char search_input_name[20];

    printf("\nWrite your search input>");
    scanf(" %6s %9s", search_input_color, search_input_name);

    for (int i = 0; i < n_shelves; i++) {
        if (strcmp(shelves[i]->item.color, search_input_color) == 0 &&
            strcmp(shelves[i]->item.name, search_input_name) == 0) {
            return shelves[i];
            }
    }
    return 0;
}

/*
int search_item(char search_input_color[7], char search_input_name[10], shelf_t shelves[], int n_shelves){
    for (int i = 0; i < n_shelves; i++){
        if (strcmp(shelves[i].item.color, search_input_color) == 0 || strcmp(shelves[i].item.name, search_input_name) == 0) {
            return i;
        }
    }

return -1;
}
 */