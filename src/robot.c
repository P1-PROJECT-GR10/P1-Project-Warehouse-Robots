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
    for (int i = 0; i < ROBOT_MAX_CAPACITY; i++){
        robot1->item[i] = (item_t){0}; //Sets every space in the robot to 0
    }

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

void manual_movement(robot_t* robot1, int* warehouse, int rows, int columns, shelf_t* shelves[], int n_shelves) {
    int i = 1;
    char word;
    printf("try moving the robot :), use: n, s, e, w, t, p or b (stop / break)\n");
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
            case 't':
                //robot_item_pickup(robot1, shelves[6], 1);
                check_nearby_shelves(robot1, shelves, n_shelves);
                break;
            default:
                printf("unreadable expression, try: n, s, e, w, t,, p or b (stop / break)\n");
                break;

        }
    }
}


void check_nearby_shelves (robot_t* robot, shelf_t* shelves[], int n_shelves){ //Der skal updateres funktionalitet i form input fra listen
    shelf_t* nearby_shelf = NULL;

    printf("Robot is on x:%d, y:%d\n", robot->x, robot->y);

    for (int i = 0; i < n_shelves; i++){
        if (shelves[i]->x == robot->x && (shelves[i]->y == robot->y-1 || shelves[i]->y == robot->y+1)){
            nearby_shelf = shelves[i];
            break;
        }
    }

    if (nearby_shelf == NULL){
        printf("Nearby shelf wasn't found):\n");
        return;
    }

    robot_item_pickup(robot, nearby_shelf, 1); //1 tallet skal nok skiftes ud med en variabel på et tidspunkt.

}

void robot_item_pickup(robot_t* robot, shelf_t* shelf, int amount) {
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

            printf("The shelf had: '%s %s', the robot is now carrying:\n", shelf->item.color, shelf->item.name);
            for (int j = 0; j <= ROBOT_MAX_CAPACITY; j++){
                if (robot->item[j].name[0]){
                    printf(" '%s %s'\n",robot->item[j].color, robot->item[j].name);
                }
            }
        } else {
            printf("Error: There were only %d items on the shelve and not the requested amount: %d\n", shelf->stock, amount);
        }
    } else{
        printf("The robot can only carry %d! The robot was requested to pick up %d items but it carried %d before", ROBOT_MAX_CAPACITY, amount, robot->number_of_items);
    }
}

void free_robot(robot_t* robot1){
    free(robot1);
}