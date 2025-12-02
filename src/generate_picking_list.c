#include "warehouse.h"
#include "generate_picking_list.h"

void generate_picking_list(item_t pickingItems[], const warehouse_t* warehouse, const int item_amount_input) {
    // Since there is 1 item per shelf n_shelves = n_items
    const int n_items = warehouse->number_of_shelves;

    for (int i = 0; i < item_amount_input; i++) {
        int random_number = rand() % n_items; // Randomness depending on the given seed.
        pickingItems[i] = warehouse->items[random_number]; // copying the whole struct
    }
}

void display_picking_list(item_t* pickingItems, int item_amount_input) {
    printf("The picking list is: ");
    for (int i = 0; i < item_amount_input; i++) {
        printf("%s %s %.2lf ", pickingItems[i].color, pickingItems[i].name, pickingItems[i].weight);
    }
    printf("\n \n");
}