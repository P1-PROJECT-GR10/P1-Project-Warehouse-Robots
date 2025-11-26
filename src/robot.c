
#include <stdio.h>
#include <stdlib.h>

#include "warehouse.h"
#include "robot.h"

robot_t* create_robot() {
    robot_t* robot1 = (robot_t*)(malloc(sizeof(robot_t)));

    robot1->robot_id = 1;
    robot1->number_of_items = 0;
    robot1->x = 9;
    robot1->y = 9;

    return robot1;
}

void move_robot(robot_t* robot1, int* warehouse, int rows, int columns, direction_e direction) {

    switch (direction) {
        case north:
            if (warehouse[columns * (robot1->y-1)  + robot1->x] == shelf
                || robot1->y < 1) {
                // checks if next move is into a shelf, or if movement is out of bounds
                //Can't move into a shelf
                printf("Error: Invalid movement\n");
                return;
                }
            warehouse[columns * robot1->y + robot1->x] = empty;

            robot1->y --;// moves the inbuilt coordinate of the robot,
                                    // and changes the robot position on the visuals
            warehouse[columns * robot1->y + robot1->x] = robot;
            break;
        case south:
            if (warehouse[columns * (robot1->y+1)  + robot1->x] == shelf
                || robot1->y >= rows - 1) {
                // checks if next move is into a shelf, or if movement is out of bounds
                //Can't move into a shelf
                printf("Error: Invalid movement\n");
                return;
                }
            warehouse[columns * robot1->y + robot1->x] = empty;
            robot1->y ++;// moves the inbuilt coordinate of the robot,
                                    // and changes the robot position on the visuals
            warehouse[columns * robot1->y + robot1->x] = robot;
            break;
        case east:
            if (warehouse[columns * robot1->y  + robot1->x+1] == shelf
                || robot1->x+1 >= columns) {
                // checks if next move is into a shelf, or if movement is out of bounds
                //Can't move into a shelf
                printf("Error: Invalid movement\n");
                return;
                }
            warehouse[columns * robot1->y + robot1->x] = empty;
            robot1->x ++; // moves the inbuilt coordinate of the robot,
                                     // and changes the robot position on the visuals
            warehouse[columns * robot1->y + robot1->x] = robot;
            break;
        case west:
            if (warehouse[columns * robot1->y  + robot1->x-1] == shelf
                || robot1->x-1 < 0) {
                // checks if next move is into a shelf, or if movement is out of bounds
                //Can't move into a shelf
                printf("Error: Invalid movement\n");
                return;
            }
            warehouse[columns * robot1->y + robot1->x] = empty;
            robot1->x --;// moves the inbuilt coordinate of the robot,
                                    // and changes the robot position on the visuals
            warehouse[columns * robot1->y + robot1->x] = robot;
            break;
        default:
            printf("Error: Invalid input\n");
            break;
    }


}

void print_robot_xy(robot_t robot1) { // for testing purposes
    printf("%d, %d\n",robot1.x, robot1.y);
}

void manual_movement(robot_t* robot1, int* warehouse, int rows, int columns) {
    int i = 1;
    char word;
    printf("try moving the robot :), use: n, s, e, w, p or b (stop / break)\n");
    while (i == 1) {
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
            case 'p':
                print_robot_xy(*robot1);
                break;
            case 'b':
                i = 0;
                break;
            default:
                printf("unreadable expression, try: n, s, e, w, p or b (stop / break)\n");
                break;

        }
    }
}