#ifndef P1_PROJECT_GENERATE_PICKING_LIST_H
#define P1_PROJECT_GENERATE_PICKING_LIST_H

#include "warehouse.h"

void generate_picking_list(item_t pickingItems[], item_t* items, int item_amount_input, int seed, int n_items);
void display_picking_list(item_t* pickingItems, int item_amount_input);

#endif //P1_PROJECT_GENERATE_PICKING_LIST_H