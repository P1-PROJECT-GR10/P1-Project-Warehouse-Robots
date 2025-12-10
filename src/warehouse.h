#pragma once

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>


//---------------------------------------MACROS---------------------------------------

/// How wide a main aisle is considered to be when generating a warehouse layout
#define MAIN_AISLE_WIDTH 1
/// How wide aa aisle is considered to be when generating a warehouse layout
#define AISLE_WIDTH 1
/// How long a shelf is considered to be when generating a warehouse layout
#define SHELF_LENGTH 2
/// How many shelves are generated when generating a warehouse layout
#define SHELF_AMOUNT 1
/// The number of items that gets generated on the picking list
#define AMOUNT_OF_PICKING_ITEMS 5
/// Stocked number of items in any given shelf
#define STOCK_AMOUNT 10
/// The maximum amount of drop zones
#define AMOUNT_OF_DROP_ZONES 2
/// The name of the file containing items
#define ITEM_FILE "items.txt"
/// Whether to optimize picking order
#define OPTIMIZE_PICKING_ORDER true

//---------------------------------------ENUMERATIONS---------------------------------------

/// Defines the state of a cell in a warehouse
typedef enum cell {empty, shelf, robot, drop_zone} cell_e;


//---------------------------------------STRUCTURES---------------------------------------

/// A structure for storing the color, name, and weight of an item.
typedef struct item {
    char color[32];  ///< String describing an items color
    char name[32];  ///< String describing an items name
    double weight;  ///< The wight of an item
} item_t;

/// A structure for storing items on a shelf. Also stores the stock of the item and the coordinates of the shelf.
typedef struct shelf {
    item_t item;    ///< The item stored in this shelf
    int stock;      ///< The stock of the item stored in this shelf
    int x;          ///< The x-coordinate of this shelf
    int y;          ///< The y-coordinate of this shelf
} shelf_t;

/// A structure for storing coordinates and dropzone activity.
typedef struct {
    int x; ///< The x-coordinate of this drop zone
    int y; ///< The x-coordinate of this drop zone
} drop_zone_t;

/// A structure for storing coordinates and dropzone activity.
typedef struct {
    drop_zone_t** zones;
    int amount;
    int max_amount;
} drop_zones;
/// A structure for storing the warehouse data
typedef struct {
    cell_e* map;
    int rows;
    int columns;
    shelf_t** shelves;
    item_t* items;
    int number_of_shelves;
    int number_of_items;
    drop_zones* drop_zones;
    bool printing;
} warehouse_t;

/// A structure for storing items in the picking list
typedef struct {
    item_t* items;
    int amount;
} picking_list_t;


//---------------------------------------FUNCTIONS---------------------------------------
void* safe_malloc(size_t size);

warehouse_t* create_warehouse();

void destroy_warehouse(warehouse_t* warehouse);

void destroy_picking_list(picking_list_t* picking_list);

/**
 * A function for calculating the manhattan distance between two nodes: (|x1 - x2| + |y1 - y2|)\n
 * Designed as heuristic function for an A* algorithm
 * @param x1 The current x-coordinate
 * @param y1 The current y-coordinate
 * @param x2 The goal x-coordinate
 * @param y2 The goal y-coordinate
 * @return An integer value representing the manhattan distance between the two input nodes
 */
int manhat_dist(int x1, int y1, int x2, int y2);

double euclidean_dist(int x1, int y1, int x2, int y2);

/**
 * Gets stored data from a cell in a warehouse.
 * @param warehouse The warehouse in which the cell exists
 * @param x x-coordinate of the cell desired cell
 * @param y y-coordinate of the cell desired cell
 * @return Pointer to desired cell
 */
cell_e* get_cell(const warehouse_t* warehouse, int x, int y);

/**
 * Checks whether a certain row is one of the top- or bottom aisles
 * @param warehouse The warehouse heap
 * @param row The row to check boundary for
 * @return Bool if row should be a top- or bottom aisle
 */
bool is_vertical_end_aisle(const warehouse_t* warehouse, int row);

/**
 * Checks whether a certain column is a main aisle
 * @param column The column to check boundary for
 * @return Bool if column should be a main aisle
 */
bool is_main_aisle(int column);

/**
 * Generates a heap allocated array representing a warehouse based on the given parameters,
 * with heap allocated structs for the shelves in the warehouse. \n
 * Use the get_cell() function for retrieving data from the warehouse.
 * @attention Free memory before returning!\n Both warehouse and shelves!
 * @param warehouse The warehouse heap
 * @return An array corresponding to a warehouse layout defined by input parameters
 */
cell_e* generate_layout(const warehouse_t* warehouse);

/**
 * Generates a heap allocated array of shelves, according to amoutn of shelves in warehouse
 * @param warehouse The warehouse heap
 * @return An array of generated shelves of struct shelf
 */
shelf_t** populate_shelves(const warehouse_t* warehouse);

/**
 * Set a cell to be of cell type drop zone
 * @param warehouse The warehouse that should be printed
 * @param x x-coordinate of the shelf
 * @param y y-coordinate of the shelf
 */
void set_drop_zone_cell(warehouse_t* warehouse, int x, int y);

/**
 * Get nearest drop zone to a point
 * @param warehouse The warehouse heap
 * @param x x-coordinate of the point to get nearest drop zone from
 * @param y y-coordinate of the point to get nearest drop zone from
 */
drop_zone_t* get_nearest_drop_zone(const warehouse_t* warehouse, int x, int y);

/**
 * Helper function for printing warehouse\n
 * Prints a graphic representation of a cells values
 * @param cell The cell that will be printed
 */
void print_cell(cell_e cell);

/**
 * Prints the entire warehouse to the terminal with visual representation of the content of the cells
 * @param warehouse The warehouse that should be printed
 */
void print_warehouse(const warehouse_t* warehouse);

/**
 * Search function that finds the shelf containing the desired item
 * @param search_input_color The color of the desired item
 * @param search_input_name The name/title of the desired item
 * @param warehouse The warehouse heap
 * @return A pointer to the shelf containing the desired item
 */
shelf_t* search_item(char search_input_color[32], char search_input_name[32], const warehouse_t* warehouse);

/**
 * A search function that prompts the user for the desired item
 * @param warehouse The warehouse heap
 * @return A pointer to the shelf containing the desired item
 */
shelf_t* manual_search_item(const warehouse_t* warehouse);

/**
 * Reads items from a file
 * @param items An array to be populated with items by the function.
 * @param number_of_items The size of the array of items, and the amount of items to be read from the file.
 * @param file The file from which the items are read
 */
int file_read_items(item_t* items, int number_of_items, FILE* file);

item_t* read_items_from_file(char* file_name, int* items_read) ;

/**
 * A helper function for generating the warehouse that populates a given shelf with an item
 * and sets the stock of the shelf while allocating memory on the heap.
 * @attention Remember to free allocated memory!
 * @param warehouse The warehouse heap
 * @param stock The stock of the item in the shelf
 * @param x x-coordinate of the shelf
 * @param y y-coordinate of the shelf
 * @return A pointer to the generated shelf
 */
struct shelf* generate_shelf(warehouse_t* warehouse, int shelf_count, int stock, int x, int y);

/**
 * A helper function for generating a struct to store the drop zones in
 * @attention Remember to free allocated memory!
 * @param capacity The maximum allowed amount of drop zones
 * @return A pointer to the generated drop zones struct
 */
drop_zones* generate_drop_zones(int capacity);

/**
 * Frees warehouse
 * @param warehouse The warehouse heap
 */
void free_warehouse(int *warehouse);

/**
 * Frees shelves
 * @param shelves An array of pointers to shelf_t structs
 * @param n_shelves Number of shelves in the warehouse
 */
void free_shelves(shelf_t** shelves, int n_shelves);

/**
 * Generates a random picking list with a given amount of items according to a set seed.
* @param pickingItems items for the picking list
*  * @param warehouse The warehouse
 * @param item_amount_input amount of items added to the picking list
 */
picking_list_t* generate_picking_list(const warehouse_t* warehouse, int item_amount_input);
/**
 * Displays the current generated picking list
 *  * @param picking_list The picking list
 * @param item_amount_input amount of items added to the picking list
 */
void display_picking_list(picking_list_t* picking_list, int item_amount_input);


/**
 * Finds nearest shelf with a given item from a list of items, to a x,y point
 * @param picking_list The picking list to remove from
 * @param warehouse The warehouse
 * @param picking_list The picking list
 */
shelf_t* find_nearest_item(int x, int y, const warehouse_t* warehouse, picking_list_t* picking_list);

/**
 * Removes item from the picking list
 * @param picking_list The picking list to remove from
 * @param item The item to remove
 */
void remove_item(picking_list_t* picking_list, item_t item);
