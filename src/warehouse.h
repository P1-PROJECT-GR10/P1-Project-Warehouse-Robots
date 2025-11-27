#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//---------------------------------------MACROS---------------------------------------

/// How wide a main aisle is considered to be when generating a warehouse layout
#define MAIN_AISLE_WIDTH 2
/// How wide aa aisle is considered to be when generating a warehouse layout
#define AISLE_WIDTH 1
/// How long a shelf is considered to be when generating a warehouse layout
#define SHELF_LENGTH 6
/// How many shelves are generated when generating a warehouse layout
#define SHELF_AMOUNT 3


//---------------------------------------ENUMERATIONS---------------------------------------

/// Defines the state of a cell in a warehouse
typedef enum cell {empty, shelf, robot} cell_e;


//---------------------------------------STRUCTURES---------------------------------------

/// A structure for storing the color, name, and weight of an item.
typedef struct item {
    char color[7];  ///< String describing an items color
    char name[10];  ///< String describing an items name
    double weight;  ///< The wight of an item
} item_t;

/// A structure for storing items on a shelf. Also stores the stock of the item and the coordinates of the shelf.
typedef struct shelf {
    item_t item;    ///< The item stored in this shelf
    int stock;      ///< The stock of the item stored in this shelf
    int x;          ///< The x-coordinate of this shelf
    int y;          ///< The y-coordinate of this shelf
} shelf_t;


//---------------------------------------FUNCTIONS---------------------------------------

/**
 * Gets stored data from a cell in a warehouse.
 * @param warehouse The warehouse in which the cell exists
 * @param columns Number of columns in the warehouse
 * @param x x-coordinate of the cell desired cell
 * @param y y-coordinate of the cell desired cell
 * @return Pointer to desired cell
 */
int* get_cell(int* warehouse, int columns, int x, int y);

/**
 * Generates a heap allocated array representing a warehouse based on the given parameters,
 * with heap allocated structs for the shelves in the warehouse. \n
 * Use the get_cell() function for retrieving data from the warehouse.
 * @attention Free memory before returning!\n Both warehouse and shelves!
 * @param main_aisle_width Width of main aisles in the warehouse.
 * @param aisle_width Width of aisles in the warehouse
 * @param shelf_length Length of shelf blocks
 * @param rows Number of rows in the warehouse
 * @param columns Number of columns in the warehouse
 * @param shelves Pointer to an array of shelf pointers
 * @param items Array of items for putting in the shelves
 * @return An array corresponding to a warehouse layout defined by input parameters
 */
int* generate_layout(int main_aisle_width, int aisle_width, int shelf_length, int rows, int columns, shelf_t* shelves[], item_t* items);

/**
 * Helper function for printing warehouse\n
 * Prints a graphic representation of a cells values
 * @param cell The cell that will be printed
 */
void print_cell(cell_e cell);

/**
 * Prints the entire warehouse to the terminal with visual representation of the content of the cells
 * @param warehouse The warehouse that should be printed
 * @param rows Number of rows in the warehouse
 * @param columns Number of columns in the warehouse
 */
void print_warehouse(int* warehouse, int rows, int columns);

/**
 * Search function that finds the shelf containing the desired item
 * @param search_input_color The color of the desired item
 * @param search_input_title The name/title of the desired item
 * @param shelves An array of pointers to the shelves you want to search through
 * @param n_shelves Number of shelves in the warehouse.
 * @return A pointer to the shelf containing the desired item
 */
shelf_t* search_item(char search_input_color[20], char search_input_title[20], shelf_t* shelves[], int n_shelves);

/**
 * A search function that prompts the user for the desired item
 * @param shelves Array of pointers to shelves in the warehouse
 * @param n_shelves Number of shelves in the warehouse
 * @return A pointer to the shelf containing the desired item
 */
shelf_t* manual_search_item(shelf_t* shelves[], int n_shelves);

/**
 * Reads items from a file
 * @param items An array to be populated with items by the function.
 * @param number_of_items The size of the array of items, and the amount of items to be read from the file.
 * @param file The file from which the items are read
 */
void file_read_items(item_t* items, int number_of_items, FILE* file);

/**
 * A helper function for generating the warehouse that populates a given shelf with an item
 * and sets the stock of the shelf while allocating memory on the heap.
 * @attention Remember to free allocated memory!
 * @param item The item that is stored in this shelf
 * @param stock The stock of the item in the shelf
 * @param x x-coordinate of the shelf
 * @param y y-coordinate of the shelf
 * @return A pointer to the generated shelf
 */
struct shelf* generate_shelf(item_t item, int stock, int x, int y);
