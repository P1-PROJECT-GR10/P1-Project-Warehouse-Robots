#pragma once
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "warehouse.h"

//---------------------------------------ENUMERATIONS---------------------------------------

#define ROBOT_MAX_CAPACITY 8
typedef enum direction {north, south, east, west, no_direction} direction_e;

//---------------------------------------STRUCTURES---------------------------------------

/**
 * @brief A structure for a robot with relevant variables and parameters.\n
 * Contains the ID, number of items, and x/y-coordinates of the robot.
 */
typedef struct robot_t {
    int number_of_items;    ///< The number of items the robot is carrying
    item_t item[ROBOT_MAX_CAPACITY]; ///< The number of items structs in an array, the robot can carry
    int x;                  ///< The x-coordinate of the robot
    int y;                  ///< The y-coordinate of the robot
    int steps;
    bool is_in_drop_zone;
} robot_t;


//---------------------------------------FUNCTIONS---------------------------------------

char* direction_to_string(direction_e direction);

/**
 * A function for creating a robot with the correct starting parameters.
 * @attention Free memory before returning!
 * @return A pointer to a heap allocated robot structure.
 */
robot_t* create_robot(const warehouse_t* warehouse);

/**
 * Test function for printing the coordinates of a robot.
 * @param robot1 A valid robot structure.
 */
void print_robot_xy(const robot_t* robot1);

/**
 * Moves a robot in a specified direction\n
 * Will print error if direction is invalid.
 * @param robot1 Robot structure that should move
 * @param warehouse Warehouse in which the robot exists
 * @param direction Direction of movement
 */
void move_robot(robot_t* robot1, const warehouse_t* warehouse, direction_e direction);

/**
 * A test function for manually moving a robot in a warehouse.
 * @param robot1 Robot structure that should move
 * @param warehouse Warehouse in which the robot exists
 * @param pickingItems The array of items of which to pick
 */
void manual_movement(robot_t* robot1, const warehouse_t* warehouse, item_t pickingItems[]);

/**
 * A function that checks for nearby shelves y+1 / y-1 from the robot
 * @param robot Pointer to the robot
 * @param robot Pointer to the warehouse
 * @param shelf Pointer to the shelf
 */
bool check_shelf(robot_t* robot, const warehouse_t* warehouse, shelf_t* shelf);

/**
 * A function that can pickup items from a shelf
 * @param robot Pointer to the robot
 * @param shelf Pointer to the shelf
 * @param amount Amount of items to pick up from shelf
 */
void robot_item_pickup(robot_t* robot, shelf_t* shelf, int amount);


/**
 * A function that empties the robot's inventory and returns the amount of items dropped as an integer
 * @param robot Pointer to the robot struct
 */
int robot_drop_all(robot_t* robot, const warehouse_t* warehouse);

/**
 * A function that returns 1 if the robot is in a valid drop zone
 * @param robot Pointer to the robot struct
 */
bool is_robot_in_drop_zone(const robot_t* robot, const warehouse_t* warehouse);

/**
 * A function that frees the robot memory allocation
 * @param robot1 Pointer to the struct robot_t that is being freed
 */
void free_robot(robot_t* robot1);