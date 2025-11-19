// Header vars
#define MAIN_AISLE_WIDTH 2
#define AISLE_WIDTH 1
#define SHELF_LENGTH 6
#define SHELF_AMOUNT 3

// Header enums
typedef enum cell {empty, shelf} cell_e;

// Functions
int* generate_layout(int main_aisle_width, int aisle_width, int shelf_length, int rows, int columns);
void print_cell(cell_e cell);
void print_warehouse(int* warehouse, int rows, int columns);
int* get_cell(int* warehouse, int columns, int x, int y);

// Types
typedef struct item {
    char name[20];
    double weight;
} item_t;

typedef struct shelf {
    item_t item;
    int stock;
    int x;
    int y;
} shelf_t;