
typedef enum cell {empty, shelf} cell_e;
typedef enum row {aisle, shelves} row_e;

#define MAIN_AISLE_WIDTH 2
#define AISLE_WIDTH 1
#define SHELF_LENGTH 6
#define SHELF_AMOUNT 3

int* generate_layout(int main_aisle_width, int aisle_width, int shelf_length, int shelves_amount, int rows, int columns);
void print_cell(cell_e cell);
void print_warehouse(int* warehouse, int rows, int columns);
int* get_cell(int* warehouse, int rows, int columns, int x, int y);