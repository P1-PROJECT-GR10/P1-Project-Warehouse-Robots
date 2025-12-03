#include "robot.h"
#include "a_star.h"
#include "bruteforce.h"

robot_t* create_robot(const warehouse_t* warehouse) {
    robot_t* robot1 = (robot_t*)(safe_malloc(sizeof(robot_t)));

    robot1->robot_id = 1;
    robot1->number_of_items = 0;
    robot1->x = 0;
    robot1->y = 0;

    int robot1_index = get_index(robot1->x, robot1->y, warehouse->columns);
    if (warehouse->map[robot1_index] == drop_zone)
        robot1->is_in_drop_zone = true;
    else
        robot1->is_in_drop_zone = false;
    warehouse->map[robot1_index] = robot; // Sets the robot in the warehouse

    return robot1;
}

char* direction_to_string(direction_e direction) {
    switch (direction) {
        case north:
            return "north";
        case south:
            return "south";
        case east:
            return "east";
        case west:
            return "west";
        default:
            return "invalid";
    }

}

void move_robot(robot_t* robot1, const warehouse_t* warehouse, const direction_e direction) {

    const int rows = warehouse->rows;
    const int columns = warehouse->columns;

    switch (direction) {
        case north:
            if (warehouse->map[columns * (robot1->y-1)  + robot1->x] == shelf
                || robot1->y < 1) {
                // checks if next move is into a shelf, or if movement is out of bounds
                //Can't move into a shelf
                printf("Error: Invalid movement\n");
                return;
                }
            if (robot1->is_in_drop_zone)
                warehouse->map[columns * robot1->y + robot1->x] = drop_zone;
            else
                warehouse->map[columns * robot1->y + robot1->x] = empty;
            robot1->y --;// moves the inbuilt coordinate of the robot, and changes the robot position on the visuals
            warehouse->map[columns * robot1->y + robot1->x] = robot;
            robot1->is_in_drop_zone = is_robot_in_drop_zone(robot1, warehouse);
            break;
        case south:
            if (warehouse->map[columns * (robot1->y+1)  + robot1->x] == shelf
                || robot1->y >= rows - 1) {
                // checks if next move is into a shelf, or if movement is out of bounds
                //Can't move into a shelf
                printf("Error: Invalid movement\n");
                return;
                }
            if (robot1->is_in_drop_zone)
                warehouse->map[columns * robot1->y + robot1->x] = drop_zone;
            else
                warehouse->map[columns * robot1->y + robot1->x] = empty;
            robot1->y ++;// moves the inbuilt coordinate of the robot, and changes the robot position on the visuals
            warehouse->map[columns * robot1->y + robot1->x] = robot;
            robot1->is_in_drop_zone = is_robot_in_drop_zone(robot1, warehouse);
            break;
        case east:
            if (warehouse->map[columns * robot1->y  + robot1->x+1] == shelf
                || robot1->x+1 >= columns) {
                // checks if next move is into a shelf, or if movement is out of bounds
                //Can't move into a shelf
                printf("Error: Invalid movement\n");
                return;
                }
            if (robot1->is_in_drop_zone)
                warehouse->map[columns * robot1->y + robot1->x] = drop_zone;
            else
                warehouse->map[columns * robot1->y + robot1->x] = empty;
            robot1->x ++; // moves the inbuilt coordinate of the robot, and changes the robot position on the visuals
            warehouse->map[columns * robot1->y + robot1->x] = robot;
            robot1->is_in_drop_zone = is_robot_in_drop_zone(robot1, warehouse);
            break;
        case west:
            if (warehouse->map[columns * robot1->y  + robot1->x-1] == shelf
                || robot1->x-1 < 0) {
                // checks if next move is into a shelf, or if movement is out of bounds
                //Can't move into a shelf
                printf("Error: Invalid movement\n");
                return;
            }
            if (robot1->is_in_drop_zone)
                warehouse->map[columns * robot1->y + robot1->x] = drop_zone;
            else
                warehouse->map[columns * robot1->y + robot1->x] = empty;
            robot1->x --;// moves the inbuilt coordinate of the robot, and changes the robot position on the visuals
            warehouse->map[columns * robot1->y + robot1->x] = robot;
            robot1->is_in_drop_zone = is_robot_in_drop_zone(robot1, warehouse);
            break;
        default:
            printf("Error: Invalid input\n");
            break;
    }
}

void print_robot_xy(const robot_t robot1) { // for testing purposes
    printf("%d, %d\n",robot1.x, robot1.y);
}

void manual_movement(robot_t* robot1, const warehouse_t* warehouse, item_t pickingItems[]) {

    const int rows = warehouse->rows;
    const int columns = warehouse->columns;

    int i = 1;
    char input;
    printf("try moving the robot :), use: n, s, e, w, p or b (stop / break)\n");
    while (input != 'b') {
        scanf("%c",&input);
        switch (input) {
            case 'n':
                move_robot(robot1, warehouse, north);
                print_warehouse(warehouse);
                break;
            case 's':
                move_robot(robot1, warehouse, south);
                print_warehouse(warehouse);
                break;
            case 'e':
                move_robot(robot1, warehouse, east);
                print_warehouse(warehouse);
                break;
            case 'w':
                move_robot(robot1, warehouse, west);
                print_warehouse(warehouse);
                break;
            case 'p':
                print_robot_xy(*robot1);
                break;
            case 'b':
                break;
            default:
                printf("unreadable expression, try: n, s, e, w, p or b (stop / break)\n");
                break;
        }
    }
}

bool check_shelf(robot_t* robot1, const warehouse_t* warehouse, shelf_t* shelf) {
    if (manhat_dist(robot1->x, robot1->y, shelf->x, shelf->y) > 1) {
        printf("The robot too far away: %d", manhat_dist(robot1->x, robot1->y, shelf->x, shelf->y));
        return false;
    }

    if (shelf->stock <= 0) {
        printf("Shelf is out of stock");
        return false;
    }

    return true;
}


void robot_item_pickup(robot_t* robot, shelf_t* shelf, const int amount) {
    if (abs(robot->x - shelf->x) != 0 || abs(robot->y - shelf->y) != 1) {
        printf("Error: Shelf isn't close enough.\n");
        return;
    }

    if (amount + robot->number_of_items > ROBOT_MAX_CAPACITY) {
        printf("The robot can only carry %d! The robot was requested to pick up %d items but it carried %d before!\n", ROBOT_MAX_CAPACITY, amount, robot->number_of_items);
        return;
    }

    if (amount <= 0) {
        printf("Error: the amount needs to be higher than 0 to pick something up.\n");
        return;
    }

    if (shelf->stock < amount) { //Checks if there is enough of the items needed on the shelf
        printf("Error: There were only %d items on the shelve and not the requested amount: %d\n", shelf->stock, amount);
        return;
    }

    shelf->stock -= amount;
    robot->number_of_items += amount; //The robot gets the item amount transferred
    printf("Robot %d picked %d item(s) from the shelve with the coordinats x: %d, y: %d.\nThe robot is now carrying %d items.\n", robot->robot_id, amount, shelf->x, shelf->y, robot->number_of_items);

    //Checks the robot's items from 0 to ROBOT_MAX_CAPACITY.
    for (int k = 0; k < amount; k++){
        for (int i = 0; i < ROBOT_MAX_CAPACITY; i++) {
            if (!robot->item[i].name[0]) {
                robot->item[i] = shelf->item; //Inserts the item struct from shelf to the robot
                break;
            }
        }
    }
    printf("The shelf had: '%s %s', the robot is now carrying:\n", shelf->item.color, shelf->item.name);
    for (int j = 0; j < ROBOT_MAX_CAPACITY; j++){
        if (robot->item[j].name[0]){
            printf(" '%s %s'\n",robot->item[j].color, robot->item[j].name);
        }
    }
}

bool is_robot_in_drop_zone(const robot_t* robot, const warehouse_t* warehouse) {
    for (int i = 0; i < warehouse->drop_zones->max_amount; i++) {
        if (robot->x != warehouse->drop_zones->zones[i]->x || robot->y != warehouse->drop_zones->zones[i]->y)
            continue; // x or y isn't corresponding to drop zone i
        return true; // Robot is in a drop zone
    }
    return false; // Robot isn't in a drop zone
}

int robot_drop_all(robot_t* robot, const warehouse_t* warehouse) {
    if (robot->number_of_items == 0) {
        printf("Robot inventory is already empty\n");
        return 0;
    }

    if (!is_robot_in_drop_zone(robot, warehouse)) {
        printf("Robot isn't in a dropzone\n");
        return 0;
    }

    int steps = 0;
    for (int i = 0; i < ROBOT_MAX_CAPACITY; i++) {
        if (robot->item[i].weight != 0 && strlen(robot->item[i].name) && strlen(robot->item[i].color)) {
            robot->item[i] = (item_t){0};
            steps++;
        }
    }
    robot->number_of_items = 0;
    printf("Robot dropped %d items off in a drop zone\n",steps);
    return steps;
}

void free_robot(robot_t* robot1){
    if (robot1 != NULL) free(robot1);
}
