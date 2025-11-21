#include <stdio.h>
#include <stdlib.h>

#include "warehouse.h"
#include "robot.h"

#define MAIN_AISLE_WIDTH 2
#define AISLE_WIDTH 1
#define SHELF_LENGTH 6
#define SHELF_AMOUNT 3

int main(void) {

    const int rows = SHELF_AMOUNT * (2 + AISLE_WIDTH) + AISLE_WIDTH;
    const int columns = (MAIN_AISLE_WIDTH * 3 + SHELF_LENGTH * 2);

    int* warehouse = generate_layout(MAIN_AISLE_WIDTH, AISLE_WIDTH, SHELF_LENGTH, SHELF_AMOUNT, rows, columns);

    robot_t* robot1 = create_robot();
    //print_robot1_id(*robot1);

    warehouse[columns * robot1->coordinate_y + robot1->coordinate_x] = robot;

    print_warehouse(warehouse, rows, columns);

    int i = 1;
    char word;
    while (i == 1) {
        //printf("try moving the robot :), use: n, s, e, w or b (stop / break)\n");
        //somehow breaks the scanf
        scanf("%c",&word);
        switch (word) {
            case 'n':
                move_robot(robot1, warehouse, rows, columns, north);
                print_warehouse(warehouse, rows, columns);
                break;
            case 's':
                move_robot(robot1, warehouse, rows, columns, south);
                print_warehouse(warehouse, rows, columns);
                break;
            case 'e':
                move_robot(robot1, warehouse, rows, columns, east);
                print_warehouse(warehouse, rows, columns);
                break;
            case 'w':
                move_robot(robot1, warehouse, rows, columns, west);
                print_warehouse(warehouse, rows, columns);
                break;
            case 'b':
                i = 0;
                break;
            default:
                printf("unreadable expression, try: n, s, e, w or b (stop / break)\n");
                break;

        }
    }
    free(warehouse);
    free(robot1);

    return 0;
}