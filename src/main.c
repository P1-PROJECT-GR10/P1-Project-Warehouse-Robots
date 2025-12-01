#include <time.h>
#include "generate_picking_list.h"
#include "warehouse.h"
#include "robot.h"
#include "a_star.h"

int main(void) {
    const int rows = SHELF_AMOUNT * (2 + AISLE_WIDTH) + AISLE_WIDTH;
    const int columns = (MAIN_AISLE_WIDTH * 3 + SHELF_LENGTH * 2);
    const int n_shelves = SHELF_AMOUNT * SHELF_LENGTH * 2 * 2;

    // Time Parameters and functionality
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    int seed = 123456789;
    srand(seed);
    int amount_of_picking_items = 5;

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

    item_t pickingItems[amount_of_picking_items];
    generate_picking_list(pickingItems, items, amount_of_picking_items, seed, n_shelves);
    display_picking_list(pickingItems, amount_of_picking_items);

    for (int i = 0; i < n_shelves; i++) {
        free(shelves[i]);
    }

    */

    robot_t* robot1 = create_robot();

    //print_robot1_id(*robot1);

    warehouse[columns * robot1->y + robot1->x] = robot; //Sets the robot in the warehouse

    print_warehouse(warehouse, rows, columns);

    // manual_movement(robot1, warehouse, rows, columns);

    // This function prints the warehouse each time for testing - this can be removed
    move_robot_to_point(robot1, warehouse, rows, columns, 14, 0);

    free(warehouse);
    free(robot1);

    // Lastly we display the runtime to see how fast it took the robot(s) to complete their assigned tasks
    clock_gettime(CLOCK_MONOTONIC, &end);
    double runtime = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    printf("Runtime: %.9f seconds\n", runtime);
    printf("Seed: %d\n", seed);

    return 0;
}