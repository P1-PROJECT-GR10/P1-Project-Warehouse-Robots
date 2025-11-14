//
// Created by magnu on 14-11-2025.
//

#include "warehouse.h"

#include <stdio.h>
#include <stdlib.h>

int** generate_layout(int main_aisle_width, int aisle_width, int shelf_length, int shelves_amount, int rows, int columns) {

    int** warehouse = (int**)malloc(sizeof(int*)*columns);

    for (int i = 0; i < columns; i++) {
        warehouse[i] = malloc(sizeof(int)*rows);
    }

    for (int i = 0; i < columns; i++) {
        for (int j = 0; j < rows; j++) {
            warehouse[i][j] = empty;
        }
    }
}

void print_cell(cell_e cell) {
    switch (cell) {
        case empty:
            printf("| |");
            break;
        case shelf:
            printf("|#|");
            break;
    }

}

void print_warehouse(int** warehouse, int rows, int columns) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            print_cell(warehouse[i][j]);
        }
        printf("\n");
    }
}


