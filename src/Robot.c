
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

void print_robot1_xy(robot_t robot1) { // for testing purposes
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
                print_robot1_xy(*robot1);
                break;
            case 'b':
                i = 0;
                break;
            case 't':
                check_nearby_shelves(robot1, shelves, n_shelves);
                break;
            default:
                printf("unreadable expression, try: n, s, e, w, p or b (stop / break)\n");
                break;

        }
    }
}


void check_nearby_shelves (robot_t* robot, shelf_t* shelves[], int n_shelves){
    shelf_t* nearby_shelf = NULL;

    printf("Robot is on x:%d, y:%d\n", robot->x, robot->y);
    printf("Shelve 2 is on x:%d, y:%d\n", shelves[2]->x, shelves[2]->y); //There is a issue that the shelves don't get their x and y correctly ): /: D: O:

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

    printf("The shelf had: '%s %s', the robot is now carrying 1x '%s'.\n", nearby_shelf->item.color, nearby_shelf->item.name, robot->item[0].name);

}

void robot_item_pickup(robot_t* robot, shelf_t* shelf, int amount) {
    if (amount <= 0) {
        printf("Error: the amount needs to be higher than 0 to pick something up.\n");
        return;
    }

    if (shelf->stock >= amount) { //Checks if there is enough of the items needed on the shelf
        shelf->stock -= amount;
        robot->number_of_items += amount; //The robot gets the item amount transferred
        printf("Robot %d picked %d item(s) from the shelve. The robot is now carrying %d items.\n", robot->robot_id, amount, robot->number_of_items);
    } else {
        printf("Error: There were only %d items on the shelve\n", shelf->stock);
    }
}