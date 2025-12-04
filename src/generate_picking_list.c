#include "warehouse.h"
#include "generate_picking_list.h"

void generate_picking_list(item_t pickingItems[], const warehouse_t* warehouse, const int item_amount_input) {
    for (int i = 0; i < item_amount_input; i++) {
        int random_number = rand() % warehouse->number_of_items; // Randomness depending on the given seed.
        pickingItems[i] = warehouse->items[random_number]; // Copying the whole struct
    }
}

void display_picking_list(item_t* pickingItems, int item_amount_input) {
    // Display the picking list in order
    printf("The picking list is: ");
    for (int i = 0; i < item_amount_input; i++) {
        printf("%s %s %.2lf ", pickingItems[i].color, pickingItems[i].name, pickingItems[i].weight);
    }
    printf("\n \n");
}