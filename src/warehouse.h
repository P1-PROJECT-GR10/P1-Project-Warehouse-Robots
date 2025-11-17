//
// Created by magnu on 14-11-2025.
//

typedef enum cell {empty, shelf} cell_e;
typedef enum row {aisle, shelves} row_e;

int* generate_layout(int main_aisle_width, int aisle_width, int shelf_length, int shelves_amount, int rows, int columns);
void print_cell(cell_e cell);
void print_warehouse(int* warehouse, int rows, int columns);
int* get_cell(int* warehouse, int rows, int columns, int x, int y);