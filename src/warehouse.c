//
// Created by magnu on 14-11-2025.
//

#include "warehouse.h"

#include <stdio.h>
#include <stdlib.h>

int* generate_layout(int main_aisle_width, int aisle_width, int shelf_length, int shelves_amount, int rows, int columns) {

    int* warehouse = (int*)malloc(sizeof(int*)*columns*rows);

    for (int i = 0; i < columns*rows; i++) {
        warehouse[i] = empty;
    }

    return warehouse;
}

void print_cell(cell_e cell) {
    switch (cell) {
        case empty:
            printf("0");
            break;
        case shelf:
            printf("1");
            break;
    }

}

int* get_cell(int* warehouse, int rows, int columns, int x, int y) {
    return &warehouse[y * columns + x];
}

void print_warehouse(int* warehouse, int rows, int columns) {
    printf("%d, %d \n", rows, columns);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            int* cell = get_cell(warehouse, rows, columns, j, i);
            print_cell(*cell);
        }
        printf("\n");
    }
}