#include "warehouse.h"

int main(void) {
    const int rows = SHELF_AMOUNT * (2 + AISLE_WIDTH) + AISLE_WIDTH;
    const int columns = (MAIN_AISLE_WIDTH * 3 + SHELF_LENGTH * 2);
    const int n_shelves = SHELF_AMOUNT * SHELF_LENGTH * 2 * 2;

    FILE* items_file = fopen("items.txt", "r");
    if (items_file == NULL) {
        printf("Failed to open file.\n");
        printf("Remember to set working directory.\n");
        exit(EXIT_FAILURE);
    }

    item_t items[n_shelves];

    int item_count = file_read_items(items, n_shelves, items_file);
    fclose(items_file);

    shelf_t* shelves[n_shelves];

    int* warehouse = generate_layout(MAIN_AISLE_WIDTH, AISLE_WIDTH, SHELF_LENGTH, rows, columns, shelves, items);

    print_warehouse(warehouse, rows, columns);

    char search_input_color[7];
    char search_input_name[10];
    int shelve_id_target;

    printf("Shelve [0] = Color: %s, Name: %s\n", shelves[0]->item.color, shelves[0]->item.name);
    printf("\nWrite your search input>");
    scanf(" %6s %9s", search_input_color, search_input_name);
    shelve_id_target = search_item(search_input_color,
                                   search_input_name,
                                   shelves,
                                   n_shelves);
    printf("The item with color %s and title %s was found in shelve id %d", search_input_color, search_input_name,shelve_id_target);


    for (int i = 0; i < n_shelves; i++) {
        free(shelves[i]);
    }

    free(warehouse);

    return 0;
}