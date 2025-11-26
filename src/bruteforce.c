#include "bruteforce.h"


void bruteforce(int* warehouse, robot_t* robot, int goal_x, int goal_y, int columns, int rows, direction_e prev) {

    if (*get_cell(warehouse, columns, goal_x, goal_y) == shelf) {
        printf("can't reach target, as it is a shelf! :(\n");
        return;
    }

    if (goal_x == robot->x && goal_y == robot->y) {
        print_warehouse(warehouse, rows, columns);
        printf("arived at destination :)\n");
        return;
    }

    neighbour_t neighbour[4];
    neighbour[0].direction = east;
    neighbour[1].direction = west;
    neighbour[2].direction = south;
    neighbour[3].direction = north;

    if (robot->x+1 >! rows
        && *get_cell(warehouse, columns, robot->x+1, robot->y) == empty
        && prev != west) {
        neighbour[0].distance = abs(goal_x - (robot->x+1)) + abs(goal_y - robot->y);
    } else {
        neighbour[0].distance = 9999;
    }
    if (robot->x-1 != -1
        && *get_cell(warehouse, columns, robot->x-1, robot->y) == empty
        && prev != east) {
        neighbour[1].distance = abs(goal_x - (robot->x-1)) + abs(goal_y - robot->y);
    } else {
        neighbour[1].distance = 9999;
    }
    if (robot->y+1 >! columns
        && *get_cell(warehouse, columns, robot->x, robot->y+1) == empty
        && prev != north) {
        neighbour[2].distance = abs(goal_x - robot->x) + abs(goal_y - (robot->y+1));
    } else {
        neighbour[2].distance = 9999;
    }
    if (robot->y-1 != -1
        && *get_cell(warehouse, columns, robot->x, robot->y-1) == empty
        && prev != south) {
        neighbour[3].distance = abs(goal_x - robot->x) + abs(goal_y - (robot->y-1));
    } else {
        neighbour[3].distance = 9999;
    }
    direction_e heading = neighbour[0].direction;
    int j = 0;
    for (int i = 1; i < 4; i++) {
        if (neighbour[j].distance > neighbour[i].distance) {
            heading = neighbour[i].direction;
            j = i;
        }
    }

    prev = heading;

    move_robot(robot, warehouse, rows, columns, heading);
    //print_warehouse(warehouse, rows, columns);
    //printf("\n");
    bruteforce(warehouse, robot, goal_x, goal_y, columns, rows, prev);
}