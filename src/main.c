#include "warehouse.h"
#include "robot.h"
#include "a_star.h"

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

    for (int i = 0; i < n_shelves; i++) {
        free(shelves[i]);
    }

    robot_t* robot1 = create_robot();
    //print_robot1_id(*robot1);

    warehouse[columns * robot1->y + robot1->x] = robot;

    print_warehouse(warehouse, rows, columns);

    manual_movement(robot1, warehouse, rows, columns);

    free(warehouse);
    free(robot1);

    return 0;
}