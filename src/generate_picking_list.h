#pragma once
#include "warehouse.h"


/**
 * Generates a random picking list with a given amount of items according to a set seed.
 * @param pickingItems items for the picking list
 * @param items items that have been fileread
 * @param item_amount_input amount of items added to the picking list
 * @param seed starting point for pseudo-generated randomness -> reusable randomness.
 * @param n_items amount of items from fileread
 */
void generate_picking_list(item_t pickingItems[], const warehouse_t* warehouse, int item_amount_input);
/**
 * Displays the current generated picking list
 * @param pickingItems items for the picking list
 * @param item_amount_input amount of items added to the picking list
 */
void display_picking_list(item_t* pickingItems, int item_amount_input);