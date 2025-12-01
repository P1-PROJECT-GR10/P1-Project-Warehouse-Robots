#include "warehouse.h"
#include "generate_picking_list.h"

void generate_picking_list(item_t pickingItems[],item_t* items, int item_amount_input, int seed, int n_items) {
    for (int i = 0; i < item_amount_input; i++) {
        int random_number = rand() % n_items; // Randomness depending on the given seed.
        pickingItems[i] = items[random_number]; // copying the whole struct
    }
}

void display_picking_list(item_t* pickingItems, int item_amount_input) {
    printf("The picking list is: ");
    for (int i = 0; i < item_amount_input; i++) {
        printf("%s %s %.2lf ", pickingItems[i].color, pickingItems[i].name, pickingItems[i].weight);
    }
    printf("\n \n");
}