//
// Created by ruben on 19-11-2025.
//

/*
Her skal der stå noget om hvordan en robot bliver lavet :).
*/

#include <stdio.h>
#include <stdlib.h>

#include "warehouse.h"

robot_t* create_robot() {

    robot_t* robot1 = (robot_t*)(malloc(sizeof(robot_t)));

    robot1->robot_id = 1;
    robot1->number_of_items = 0;
    robot1->coordinate_x = 9;
    robot1->coordinate_y = 9;

    return robot1;
}

void print_robot1_id(robot_t robot1) { // for testing purposes
    printf("%d",robot1.robot_id);
}

void move_robot(robot_t* robot, int dx, int dy, int* warehouse, int rows, int columns, direction_e direction) {
    int new_x = robot->coordinate_x + dx;
    int new_y = robot->coordinate_y + dy;

    //Bounds
    if (new_x >= columns || new_y >= rows)
        return;

    if (warehouse[new_y * columns + new_x] == shelf) //Can't move into a shelf
        return;

    robot->coordinate_x = new_x;
    robot->coordinate_y = new_y;

    switch (direction) {
        case 'north':
            if ( new_x >= columns || new_y >= rows)
                return;

            if (warehouse[new_y * columns + new_x] == shelf) //Can't move into a shelf
                return;

            robot->coordinate_y --;
            break;
        case 'south':
            if (new_x >= columns || new_y >= rows)
                return;

            if (warehouse[new_y * columns + new_x] == shelf) //Can't move into a shelf
                return;

            robot->coordinate_y ++;
            break;
        case 'east':
            if (new_x >= columns || new_y >= rows)
                return;

            if (warehouse[new_y * columns + new_x] == shelf) //Can't move into a shelf
                return;

            robot->coordinate_x ++;
            break;
        case 'west':
            if (new_x >= columns || new_y >= rows)
                return;

            if (warehouse[new_y * columns + new_x] == shelf) //Can't move into a shelf
                return;

            robot->coordinate_x --;
            break;
        default:
            printf("Error: Invalid input");
            break;
    }
}