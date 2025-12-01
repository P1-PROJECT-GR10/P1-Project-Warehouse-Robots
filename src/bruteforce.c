#include "bruteforce.h"
#include "a_star.h"

int manhat_dist(int x, int y, int goal_x, int goal_y) {
    int i = abs(goal_x - x) + abs(goal_y - y);
    return i;
}

int get_mirror_direction(neighbour_t neighbour) {
    switch (neighbour.direction) {
        case north:
            return south;
        case south:
            return north;
        case east:
            return west;
        case west:
            return east;
        default:
            printf("wrong input in funktion: get_mirror_direction");
            return -1;
    }
}

bool is_valid(int x, int y, int rows, int columns) {
    if (x >= 0 && x < columns && y >= 0 && y < rows) {
        return true;
    } else {
        return false;
    }
}

void bruteforce(int* warehouse, robot_t* robot, int goal_x, int goal_y, int columns, int rows) {

    // checks if goal point is a shelf, i.e. impassible.
    if (*get_cell(warehouse, columns, goal_x, goal_y) == shelf) {
        printf("can't reach target, as it is a shelf! :(\n");
        return;
    }

    // checks if goal point is out of bounds, i.e. impassible.
    if (goal_x < 0 || goal_x > columns || goal_y < 0 || goal_y > rows) {
        printf("can't reach target, as it is out of bounds! :(\n");
        return;
    }

    // calls the recoursive algorithm, and prints the amount of recoursive calls.
    printf("\n");
    int amount_of_moves = bruteforce_recursive(warehouse, robot, goal_x, goal_y, columns, rows, -1, 0);
    printf("robot moved: %d tiles\n", amount_of_moves);
}

int bruteforce_recursive(int* warehouse, robot_t* robot, int goal_x, int goal_y, int columns, int rows, direction_e prev, int moves) {

    if (goal_x == robot->x && goal_y == robot->y) {
        print_warehouse(warehouse, rows, columns);
        printf("arived at destination :)\n");
        return moves;
    } // checks if the robot has arrived at the goal point.

    neighbour_t neighbour[4]; // initializes the neighbours for the robot.
    //Their directions:
    neighbour[0].direction = east;
    neighbour[1].direction = west;
    neighbour[2].direction = south;
    neighbour[3].direction = north;

    //and their positions compared to the robots position:
    //east:
    neighbour[0].x = robot->x+1;
    neighbour[0].y = robot->y;
    // west:
    neighbour[1].x = robot->x-1;
    neighbour[1].y = robot->y;
    // south:
    neighbour[2].x = robot->x;
    neighbour[2].y = robot->y+1;
    // north:
    neighbour[3].x = robot->x;
    neighbour[3].y = robot->y-1;


    // Evaluates the manhattan distance to the goal point from each neighbouring points individually.
    // If the point is unreachable, then its distance is set to infinity, so that it never is the closest point.
    for (int i = 0; i < 4; i++) {
        // initializes the status of the neighbouring cell.
        neighbour[i].cell = *get_cell(warehouse, columns, neighbour[i].x, neighbour[i].y);
        if (is_valid(neighbour[i].x, neighbour[i].y, rows, columns) == true // is neighbour within bounds?
            && neighbour[i].cell == empty // is the neighbour an empty cell?
            // to prevent the robot from moving back to were it just came from.
            && prev != get_mirror_direction(neighbour[i])) {
            //calculates manhattan distance to goal point.
            neighbour[i].distance = manhat_dist(neighbour[i].x, neighbour[i].y, goal_x, goal_y);
        } else {
            neighbour[i].distance = INFINITY; // if not reachable, make distance infinite. >:)
        }
    }

    // The 0'th direction (east) is base case and all others are evaluated from there.
    direction_e heading = neighbour[0].direction;

    // current = currently used direction. i = Next direction to compare to, to see if it is closer to the goal.
    int current = 0;
    for (int i = 1; i < 4; i++) {
        if (neighbour[current].distance > neighbour[i].distance) {
            heading = neighbour[i].direction;
            current = i;
        }
    }
    // moves robot in the direction that has been chosen.
    move_robot(robot, warehouse, rows, columns, heading);

    print_warehouse(warehouse, rows, columns); // Can print each individual step,
    printf("%d moves \n  \n", moves);        // nice for testing purposes.

    prev = heading; // prepares the function for the next iteration.

    // executes the next step.
    return bruteforce_recursive(warehouse, robot, goal_x, goal_y, columns, rows, prev, moves + 1);
}
