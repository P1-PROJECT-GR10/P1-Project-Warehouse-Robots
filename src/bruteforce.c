#include "bruteforce.h"

void bruteforce(int* warehouse, robot_t* robot, int goal_x, int goal_y, int columns, int rows) {

    if (*get_cell(warehouse, columns, goal_x, goal_y) == shelf) {
        printf("can't reach target, as it is a shelf! :(\n");
        return;
    } // checks if goal point is a shelf, i.e. impassible.

    if (goal_x < 0 || goal_x > columns || goal_y < 0 || goal_y > rows) {
        printf("can't reach target, as it is out of bounds! :(\n");
        return;
    } // checks if goal point is out of bounds, i.e. impassible.

    printf("\n");
    int amount_of_moves = bruteforce_recoursive(warehouse, robot, goal_x, goal_y, columns, rows, -1, 0);
    printf("robot moved: %d tiles\n", amount_of_moves);
} // calls the recoursive algorithm, and prints the amount of recoursive calls.

int bruteforce_recoursive(int* warehouse, robot_t* robot, int goal_x, int goal_y, int columns, int rows, direction_e prev, int moves) {

    if (goal_x == robot->x && goal_y == robot->y) {
        print_warehouse(warehouse, rows, columns);
        printf("arived at destination :)\n");
        return moves;
    } // checks if the robot has arrived at the goal point.

    neighbour_t neighbour[4]; // initializes the four possible moves.
    neighbour[0].direction = east;
    neighbour[1].direction = west;
    neighbour[2].direction = south;
    neighbour[3].direction = north;

    // Evaluates the manhattan distance to the goal point from each neighbouring points individually.
    // If the point is unreachable, then its distance is set to 9999, so that never is the closest point.
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

    direction_e heading = neighbour[0].direction; // The 0'th direction (east) is base case,
                                                  // and all others are evaluated from there.

    int j = 0;
    for (int i = 1; i < 4; i++) {
        if (neighbour[j].distance > neighbour[i].distance) {
            heading = neighbour[i].direction;
            j = i;
        }
    } // j = current used direction. i = Next direction to compare to, to see if it is closer to the goal.

    move_robot(robot, warehouse, rows, columns, heading); // moves robot in the direction that has been chosen.

    // print_warehouse(warehouse, rows, columns); // Can print each individual step,
    // printf("%d \n moves \n", distance);        // nice for testing purposes.

    prev = heading; // prepares the function for the next iteration.

    return bruteforce_recoursive(warehouse, robot, goal_x, goal_y, columns, rows, prev, moves + 1);
    // executes the next step.
}