#include "warehouse.h"
#include "generate_picking_list.h"

// Der skal seedes i main -> srand(seed) så det en global seed

void generate_picking_list(item_t pickingItems[],item_t* items, shelf_t* shelves[], int item_amount_input, int seed, int n_items) {

    for (int i = 0; i < item_amount_input; i++) {
        int random_number = rand() % n_items;

        pickingItems[i] = items[random_number]; // copying the whole struct
    }
}

void stock_random_shelves(item_t* pickingItems, shelf_t* shelves[], int item_amount_input, int seed, int n_shelves) {
    for (int i = 0; i < item_amount_input; i++) {
        int random_number = rand() % n_shelves;

        shelves[random_number]->item = pickingItems[i];
        shelves[random_number]->stock++;
        // int temp_x = shelves[random_number]->x; Det kan være man vil vide hvilke shelves der har item
        // int temp_y = shelves[random_number]->y; Det kan være man vil vide hvilke shelves der har item
    }
}

void display_picking_list(item_t* pickingItems, shelf_t* shelves[], int item_amount_input) {
    for (int i = 0; i < item_amount_input; i++) {
        printf("The picking list is: %s %s %lf \n", pickingItems[i].color, pickingItems[i].name, pickingItems[i].weight);
    }
}

/*
 * void display_stocked_shelves() {
 *
 * }
 */