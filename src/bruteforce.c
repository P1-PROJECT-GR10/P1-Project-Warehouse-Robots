#include "bruteforce.h"
#include "warehouse.h"

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
            printf("Wrong input in function: get_mirror_direction");
            return no_direction;
    }
}

void bruteforce_algorithm(const warehouse_t* warehouse, robot_t* robot, int goal_x, int goal_y) {
    // checks if goal point is a shelf, i.e. impassible.
    if (*get_cell(warehouse, goal_x, goal_y) == shelf) {
        printf("Can't reach target, as it is a shelf! :(\n");
        return;
    }

    // checks if goal point is out of bounds, i.e. impassible.
    if (!is_in_bounds(goal_x, goal_y, warehouse)) {
        printf("Can't reach target, as it is out of bounds! :(\n");
        return;
    }

    // calls the recursive algorithm, and prints the amount of recursive calls.
    int moves = 0;
    int amount_of_moves = bruteforce_recursive(warehouse, robot, goal_x, goal_y, no_direction, moves);
    printf("\nrobot moved: %d tiles\n", amount_of_moves);
}

void bruteforce_get_picking_list(robot_t* robot1, const warehouse_t* warehouse, picking_list_t* picking_list) {
    for (int i = 0; i < picking_list->amount; i++) {
        shelf_t* goal_shelf;
        if (OPTIMIZE_PICKING_ORDER) {
            goal_shelf = find_nearest_item(robot1->x, robot1->y, warehouse, picking_list);
        } else {
            goal_shelf = search_item(picking_list->items[i].name, picking_list->items[i].color, warehouse);
        }

        int goal_x = goal_shelf->x;
        int goal_y;

        int index = get_index(goal_shelf->x, goal_shelf->y+1, warehouse->columns);
        if (warehouse->map[index] == empty) {
            goal_y = goal_shelf->y + 1;
        } else {
            goal_y = goal_shelf->y - 1;
        }

        printf("Item %d found at shelf x: %d, y: %d\n"
               "Navigating to (%d, %d)\n", i+1, goal_shelf->x, goal_shelf->y, goal_x, goal_y);

        bruteforce_algorithm(warehouse, robot1, goal_x, goal_y);
        if (check_shelf(robot1, warehouse, goal_shelf)) {
            robot_item_pickup(robot1, goal_shelf, 1);
            remove_item(picking_list, goal_shelf->item);
        }
    }
    //move_robot_to_point(robot1, warehouse, 9, 9); // Move robot back to (9, 9) or a dropzone
    drop_zone_t* drop_zone = get_nearest_drop_zone(warehouse, robot1->x, robot1->y);
    printf("Robot is finished picking up items, navigating to drop zone at %d, %d\n",drop_zone->x, drop_zone->y);

    bruteforce_algorithm(warehouse, robot1, drop_zone->x, drop_zone->y);

    printf("Robot navigated to drop zone\n");
    robot_drop_all(robot1, warehouse);
    printf("Robot drops items at (%d %d)\n", drop_zone->x, drop_zone->y);
    printf("Robot is finished, and took %d steps in total.\n\n",robot1->steps);
}

int bruteforce_recursive(const warehouse_t* warehouse, robot_t* robot, const int goal_x, const int goal_y, direction_e prev, int moves) {
    if (goal_x == robot->x && goal_y == robot->y) {
        print_warehouse(warehouse);
        printf("Arrived at destination :)\n");
        return moves;
    } // checks if the robot has arrived at the goal point.

    neighbour_t neighbour[4]; // initializes the neighbours for the robot.

    //The directions:           north,      south,      east,     west;
    int directions[4][2] = {{0, -1}, {0, 1}, {1, 0}, {-1, 0}};
    for (int i = north; i <= west; i++) {
        // sets the direction the neighbour is, relative to the robots position.
        neighbour[i].direction = i;
        // sets the x and y - coordinates for the neighbouring cell.
        neighbour[i].x = directions[i][0] + robot->x;
        neighbour[i].y = directions[i][1] + robot->y;
        // initializes the status of the neighbouring cell.
        neighbour[i].cell = *get_cell(warehouse, neighbour[i].x, neighbour[i].y);
    }

    // Evaluates the manhattan distance to the goal point from each neighbouring points individually.
    // If the point is unreachable, then its distance is set to infinity, so that it never is the closest point.
    for (int i = north; i <= west; i++) {

        if (is_in_bounds(neighbour[i].x, neighbour[i].y, warehouse) == true // is neighbour within bounds?
            && (neighbour[i].cell == empty || neighbour[i].cell == drop_zone) // is the neighbour an empty cell?
            // to prevent the robot from moving back to where it just came from.
            && prev != get_mirror_direction(neighbour[i])) {
            // calculates distance to goal point.
            neighbour[i].distance = (int)euclidean_dist(neighbour[i].x, neighbour[i].y, goal_x, goal_y);
            // neighbour[i].distance = manhat_dist(neighbour[i].x, neighbour[i].y, goal_x, goal_y);
        } else {
            neighbour[i].distance = INFINITY; // if not reachable, make distance infinite.
        }
    }

    // The 0'th direction (north) is base case and all others are evaluated from there.
    direction_e heading = neighbour[0].direction;

    // current = currently used direction. i = Next direction to compare to, to see if it is closer to the goal.
    int current = north;
    for (int i = north; i <= west; i++) {
        if (neighbour[current].distance > neighbour[i].distance) {
            heading = neighbour[i].direction;
            current = i;
        }
    }

    // moves robot in the direction that has been chosen.
    move_robot(robot, warehouse, heading);

    // print_warehouse(warehouse); // Can print each individual step,
    // printf("%d moves \n  \n", moves);        // nice for testing purposes.

    prev = heading; // prepares the function for the next iteration.

    // executes the next step.
    return bruteforce_recursive(warehouse, robot, goal_x, goal_y, prev, moves + 1);
}
