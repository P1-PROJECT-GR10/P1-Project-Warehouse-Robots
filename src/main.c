#include <time.h>
#include "generate_picking_list.h"
#include "warehouse.h"
#include "robot.h"

int main(void) {
    const int rows = SHELF_AMOUNT * (2 + AISLE_WIDTH) + AISLE_WIDTH;
    const int columns = (MAIN_AISLE_WIDTH * 3 + SHELF_LENGTH * 2);
    const int n_shelves = SHELF_AMOUNT * SHELF_LENGTH * 2 * 2;

    int seed = time(NULL);
    srand(seed);

    FILE* items_file = fopen("items.txt", "r");
    if (items_file == NULL) {
        printf("Failed to open file.\n");
        printf("Remember to set working directory.\n");
        exit(EXIT_FAILURE);
    }

    item_t items[n_shelves];

    file_read_items(items, n_shelves, items_file);

    fclose(items_file);

    shelf_t* shelves[n_shelves];

    int* warehouse = generate_layout(MAIN_AISLE_WIDTH, AISLE_WIDTH, SHELF_LENGTH, rows, columns, shelves, items);


    /*

    // Below is code for testing search functionality. Uncomment above to test.

    for (int i = 0; i < n_shelves; i++) {
        printf("[%d] %s %s\n", i, shelves[i]->item.color, shelves[i]->item.name);
    }

    char search_input_color[10] = "purple";
    char search_input_title[10] = "tv";

    shelf_t shelf_target_auto = *search_item(search_input_color, search_input_title, shelves, n_shelves);

    printf("\nAuto search found: %s %s, at x: %d y: %d\n", shelf_target_auto.item.color, shelf_target_auto.item.name, shelf_target_auto.x, shelf_target_auto.y);

    shelf_t shelf_target_manual = *manual_search_item(shelves, n_shelves);
    printf("the item in the shelf is %s %s\n", shelf_target_manual.item.color, shelf_target_manual.item.name);
    printf("The specified item was found at x: %d  y: %d\n", shelf_target_manual.x, shelf_target_manual.y);

    */
    item_t pickingItems[AMOUNT_OF_PICKING_ITEMS];
    generate_picking_list(pickingItems, items, AMOUNT_OF_PICKING_ITEMS, seed, n_shelves);
    display_picking_list(pickingItems, AMOUNT_OF_PICKING_ITEMS);

    shelf_t picking_shelves[AMOUNT_OF_PICKING_ITEMS];
    for (int i = 0; i < AMOUNT_OF_PICKING_ITEMS; i++){
        picking_shelves[i] = *search_item(pickingItems[i].color,pickingItems[i].name, shelves, n_shelves);

    }

    //shelf_t shelf_target_auto = *search_item(search_input_color, search_input_title, shelves, n_shelves);

    //printf("\nAuto search found: %s %s, at x: %d y: %d\n", shelf_target_auto.item.color, shelf_target_auto.item.name, shelf_target_auto.x, shelf_target_auto.y);


    robot_t* robot1 = create_robot();
    //print_robot1_id(*robot1);

    warehouse[columns * robot1->y + robot1->x] = robot; //Sets the robot in the warehouse

    print_warehouse(warehouse, rows, columns);

    manual_movement(robot1, warehouse, rows, columns, shelves, n_shelves, pickingItems);

    /*
    free_warehouse(warehouse);
    free_robot(robot1);
    free_shelves(shelves, n_shelves);
     */

    return 0;
}
