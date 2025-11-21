
#include <stdio.h>
#include <stdlib.h>

#include "warehouse.h"
#include "robot.h"

robot_t* create_robot() {

    robot_t* robot1 = (robot_t*)(malloc(sizeof(robot_t)));

    robot1->robot_id = 1;
    robot1->number_of_items = 0;
    robot1->coordinate_x = 9;
    robot1->coordinate_y = 9;

    return robot1;
}

void move_robot(robot_t* robot, int* warehouse, int rows, int columns, direction_e direction) {

    switch (direction) {
        case north:
            if (warehouse[columns * (robot->coordinate_y-1)  + robot->coordinate_x] == shelf
                || robot->coordinate_y < 0) {
                // checks if next move is into a shelf, or if movement is out of bounds
                //Can't move into a shelf
                printf("Error: Invalid movement");
                return;
                }
            robot->coordinate_y --;
            break;
        case south:
            if (warehouse[columns * (robot->coordinate_y+1)  + robot->coordinate_x] == shelf
                || robot->coordinate_y > columns) {
                // checks if next move is into a shelf, or if movement is out of bounds
                //Can't move into a shelf
                printf("Error: Invalid movement");
                return;
                }
            robot->coordinate_y ++;
            break;
        case east:
            if (warehouse[columns * robot->coordinate_y  + robot->coordinate_x+1] == shelf
                || robot->coordinate_x+1 > rows) {
                // checks if next move is into a shelf, or if movement is out of bounds
                //Can't move into a shelf
                printf("Error: Invalid movement");
                return;
                }
            robot->coordinate_x ++;
            break;
        case west:
            if (warehouse[columns * robot->coordinate_y  + robot->coordinate_x-1] == shelf
                || robot->coordinate_x-1 < 0) {
                // checks if next move is into a shelf, or if movement is out of bounds
                //Can't move into a shelf
                printf("Error: Invalid movement");
                return;
            }
            robot->coordinate_x --;
            break;
        default:
            printf("Error: Invalid input");
            break;
    }
}

void print_robot1_id(robot_t robot1) { // for testing purposes
    printf("%d",robot1.robot_id);
}
