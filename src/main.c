#include <stdio.h>
#include <stdlib.h>

#include "warehouse.h"

#define MAIN_AISLE_WIDTH 2
#define AISLE_WIDTH 1
#define SHELF_LENGTH 6
#define SHELF_AMOUNT 3

int main(void) {

    const int rows = SHELF_AMOUNT * (2 + AISLE_WIDTH) + AISLE_WIDTH;
    const int columns = (MAIN_AISLE_WIDTH * 3 + SHELF_LENGTH * 2);

    int* warehouse = generate_layout(MAIN_AISLE_WIDTH, AISLE_WIDTH, SHELF_LENGTH, SHELF_AMOUNT, rows, columns);

    print_warehouse(warehouse, rows, columns);

    robot_t* robot1 = create_robot();
    //print_robot1_id(*robot1);


    free(warehouse);
    free(robot1);

    return 0;
}