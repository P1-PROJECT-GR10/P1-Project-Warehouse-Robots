#include "robot.h"
#include "a_star.h"

robot_t* create_robot(const warehouse_t* warehouse) {
    robot_t* robot1 = (robot_t*)(safe_malloc(sizeof(robot_t)));

    robot1->robot_id = 1;
    robot1->number_of_items = 0;
    robot1->x = 0;
    robot1->y = 0;

    int robot1_index = get_index(robot1->x, robot1->y, warehouse->columns);
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
            warehouse->map[columns * robot1->y + robot1->x] = empty;

            robot1->y --;// moves the inbuilt coordinate of the robot,
                                    // and changes the robot position on the visuals
            warehouse->map[columns * robot1->y + robot1->x] = robot;
            break;
        case south:
            if (warehouse->map[columns * (robot1->y+1)  + robot1->x] == shelf
                || robot1->y >= rows - 1) {
                // checks if next move is into a shelf, or if movement is out of bounds
                //Can't move into a shelf
                printf("Error: Invalid movement\n");
                return;
                }
            warehouse->map[columns * robot1->y + robot1->x] = empty;
            robot1->y ++;// moves the inbuilt coordinate of the robot,
                                    // and changes the robot position on the visuals
            warehouse->map[columns * robot1->y + robot1->x] = robot;
            break;
        case east:
            if (warehouse->map[columns * robot1->y  + robot1->x+1] == shelf
                || robot1->x+1 >= columns) {
                // checks if next move is into a shelf, or if movement is out of bounds
                //Can't move into a shelf
                printf("Error: Invalid movement\n");
                return;
                }
            warehouse->map[columns * robot1->y + robot1->x] = empty;
            robot1->x ++; // moves the inbuilt coordinate of the robot,
                                     // and changes the robot position on the visuals
            warehouse->map[columns * robot1->y + robot1->x] = robot;
            break;
        case west:
            if (warehouse->map[columns * robot1->y  + robot1->x-1] == shelf
                || robot1->x-1 < 0) {
                // checks if next move is into a shelf, or if movement is out of bounds
                //Can't move into a shelf
                printf("Error: Invalid movement\n");
                return;
            }
            warehouse->map[columns * robot1->y + robot1->x] = empty;
            robot1->x --;// moves the inbuilt coordinate of the robot,
                                    // and changes the robot position on the visuals
            warehouse->map[columns * robot1->y + robot1->x] = robot;
            break;
        default:
            printf("Error: Invalid input\n");
            break;
    }
}

void print_robot_xy(const robot_t* robot1) { // for testing purposes
    printf("%d, %d\n",robot1->x, robot1->y);
}

void manual_movement(robot_t* robot1, const warehouse_t* warehouse, item_t pickingItems[]) {

    const int rows = warehouse->rows;
    const int columns = warehouse->columns;

    int i = 1;
    char input;
    printf("try moving the robot :), use: n, s, e, w, t, p or b (stop / break)\n");
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
                print_robot_xy(robot1);
                break;
            case 'b':
                break;
            case 't':
                //robot_item_pickup(robot1, shelves[6], 1);
                check_nearby_shelves(robot1, warehouse, pickingItems);
                break;
            default:
                printf("unreadable expression, try: n, s, e, w, t, p or b (stop / break)\n");
                break;
        }
    }
}


void check_nearby_shelves (robot_t* robot, const warehouse_t* warehouse, item_t pickingItems[]){
    shelf_t* nearby_shelf[] = {NULL,NULL}; //Since there is only maximum 2 nearby_shelf

    const int n_shelves = warehouse->number_of_shelves;

    printf("Robot is on x:%d, y:%d\n", robot->x, robot->y);

    for (int i = 0, index = 0; i < n_shelves && index < 2; i++){
        if (warehouse->shelves[i]->x == robot->x &&
           (warehouse->shelves[i]->y == robot->y-1 ||
            warehouse->shelves[i]->y == robot->y+1)){ // Finds the nearby shelves around the robot
            nearby_shelf[index] = warehouse->shelves[i];
            index++;
        }
    }

    if (nearby_shelf[0] == NULL && nearby_shelf[1] == NULL){
        printf("Nearby shelf wasn't found):\n"); //Prints if there weren't found at least one shelf
        return;
    }
    for (int n = 0; n < AMOUNT_OF_PICKING_ITEMS; n++){ //Runs the for loop AMOUNT_OF_PICKING_ITEMS times
        for (int shelfID = 0; shelfID <= 1; shelfID++){ //Checks both nearby_shelves pr. AMOUNT_OF_PICKING_ITEMS
            if (nearby_shelf[shelfID] != NULL){ //Checks the nearby_shelf is valid and has being assigned
                if (strcmp(pickingItems[n].name, nearby_shelf[shelfID]->item.name) == 0 &&
                    strcmp(pickingItems[n].color, nearby_shelf[shelfID]->item.color) == 0){ //Now checks if the nearby_shelves has an item in the picking list
                    pickingItems[n] = (item_t){0}; //Sets the pickingItems to 0, therefore the item gets removed since it is being picked up.
                    printf("The picking list is: ");
                    for (int i = 0; i < AMOUNT_OF_PICKING_ITEMS; i++) {
                        printf("%s %s %.2lf ", pickingItems[i].color, pickingItems[i].name, pickingItems[i].weight);
                    }
                    printf("\n \n");

                    robot_item_pickup(robot, nearby_shelf[shelfID], 1); //Runs pickup function
                }
            }
        }
    }
}


void robot_item_pickup(robot_t* robot, shelf_t* shelf, const int amount) {

    if (abs(robot->x - shelf->x) != 0 || abs(robot->y - shelf->y) != 1) {
        printf("Error: Shelf isn't close enough.\n");
        return;
    }

    if (amount + robot->number_of_items <= ROBOT_MAX_CAPACITY){
        if (amount <= 0) {
            printf("Error: the amount needs to be higher than 0 to pick something up.\n");
            return;
        }

        if (shelf->stock >= amount) { //Checks if there is enough of the items needed on the shelf
            shelf->stock -= amount;
            robot->number_of_items += amount; //The robot gets the item amount transferred
            printf("Robot %d picked %d item(s) from the shelve with the coordinats x: %d, y: %d. The robot is now carrying %d items.\n", robot->robot_id, amount, shelf->x, shelf->y, robot->number_of_items);

            //Checks the robot's items from 0 to ROBOT_MAX_CAPACITY.
            for (int k = 0; k < amount; k++){
                for (int i = 0; i < ROBOT_MAX_CAPACITY; i++) {
                    if (!robot->item[i].name[0]) {
                        robot->item[i] = shelf->item; //Inserts the item struct from shelf to the robot
                        break;
                    }
                }
            }
            printf("The shelf had: '%s %s', the robot is now carrying:", shelf->item.color, shelf->item.name);
            for (int j = 0; j < ROBOT_MAX_CAPACITY; j++){
                if (robot->item[j].name[0]){
                    printf(" '%s %s'\n",robot->item[j].color, robot->item[j].name);
                }
            }
        } else {
            printf("Error: There were only %d items on the shelve and not the requested amount: %d\n", shelf->stock, amount);
        }
    } else{
        printf("The robot can only carry %d! The robot was requested to pick up %d items but it carried %d before!\n", ROBOT_MAX_CAPACITY, amount, robot->number_of_items);
    }
}

bool is_robot_in_drop_zone(const robot_t* robot, const warehouse_t* warehouse) {
    for (int i = 0; i < warehouse->drop_zones->capacity; i++) {
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
        if (robot->item[i].weight != 0 && !strlen(robot->item[i].name) && !strlen(robot->item[i].color)) {
            robot->item[i] = (item_t){0};
            steps++;
        }
    }
    robot->number_of_items = 0;
    printf("Robot dropped %d items off in a drop zone\n",steps);
    return steps;
}

void free_robot(robot_t* robot1){
    free(robot1);
}
