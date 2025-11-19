#include "warehouse.h"

#include <stdio.h>
#include <stdlib.h>

int* generate_layout(int main_aisle_width, int aisle_width, int shelf_length, int rows, int columns) {
    int* warehouse = (int*)malloc(sizeof(int*)*columns*rows);

    for (int i = 0, l = 0; i < rows; i++) {
        // i: Row count
        // l: Working column count
        if (i > aisle_width-1 && i < rows-aisle_width) { // If i is inbetween top & bottom aisle boundary, l++
            l++;
        }
        for (int j = i * columns, k = 0; j < i * columns + columns; j++, k++) {
            // j: True array position
            if (i <= aisle_width-1 || i >= rows-aisle_width) { // Top & bottom aisle boundary
                warehouse[j] = empty; // Top & bottom aisle
            } else { // Generate the rows inbetween top & bottom aisles
                if (k < main_aisle_width || k >= main_aisle_width*2+shelf_length*2 || // Side main aisle boundaries
                    k >= main_aisle_width+shelf_length && k < main_aisle_width+shelf_length+main_aisle_width) { // Main aisle boundary
                    warehouse[j] = empty; // Main aisle
                } else {
                    if (l > aisle_width + 2) {l = 1;} // If l > aisle_width + shelf_width, reset working column count
                    if (l <= 2) { // If l <= shelf_width, place shelf
                        warehouse[j] = shelf;
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
    }

}

int* get_cell(int* warehouse, int columns, int x, int y) {
    return &warehouse[y * columns + x];
}

void print_warehouse(int* warehouse, int rows, int columns) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            int* cell = get_cell(warehouse, columns, j, i);
            print_cell(*cell);
            if (j == columns-1) {
                printf("|"); // Print '|' at end of row
            }
        }
        printf("\n");
    }
}

shelf_t generate_shelf(item_t item, int stock, int x, int y) {
    shelf_t shelf;
    shelf.item = item;
    shelf.stock = stock;
    shelf.x = x;
    shelf.y = y;
}

item_t generate_item(char name[20], double weight) {
    item_t item;
    // item.name = name[]; How? Use string library?
    item.weight = weight;
}