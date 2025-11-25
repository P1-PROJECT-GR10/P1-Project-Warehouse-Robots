#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Header vars
#define MAIN_AISLE_WIDTH 2
#define AISLE_WIDTH 1
#define SHELF_LENGTH 6
#define SHELF_AMOUNT 3

// Header enums
typedef enum cell {empty, shelf} cell_e;

// Types
typedef struct item {
    char color[7];
    char name[10];
    double weight;
} item_t;

typedef struct shelf {
    item_t item;
    int stock;
    int x;
    int y;
} shelf_t;

// Functions
int* generate_layout(int main_aisle_width, int aisle_width, int shelf_length, int rows, int columns, shelf_t* shelves[], item_t* items);
void print_cell(cell_e cell);
void print_warehouse(int* warehouse, int rows, int columns);
int* get_cell(int* warehouse, int columns, int x, int y);
shelf_t* search_item(char search_input_color[7], char search_input_title[10], shelf_t* shelves[], int n_shelves);

int file_read_items(item_t* items, int number_of_items, FILE* file);
struct shelf* generate_shelf(item_t item, int stock, int x, int y);