#pragma once

#include "warehouse.h"

//---------------------------------------ENUMERATIONS---------------------------------------

#define ROBOT_MAX_CAPACITY 8
typedef enum direction {north, south, east, west} direction_e;

//---------------------------------------STRUCTURES---------------------------------------

/**
 * @brief A structure for a robot with relevant variables and parameters.\n
 * Contains the ID, number of items, and x/y-coordinates of the robot.
 */
typedef struct robot_t {
    int robot_id;           ///< The ID of the robot
    int number_of_items;    ///< The number of items the robot is carrying
    item_t item[ROBOT_MAX_CAPACITY]; ///< The number of items structs in an array, the robot can carry
    int x;                  ///< The x-coordinate of the robot
    int y;                  ///< The y-coordinate of the robot
} robot_t;


//---------------------------------------FUNCTIONS---------------------------------------

char* direction_to_string(direction_e direction);

/**
 * A function for creating a robot with the correct starting parameters.
 * @attention Free memory before returning!
 * @return A pointer to a heap allocated robot structure.
 */
robot_t* create_robot();

/**
 * Test function for printing the coordinates of a robot.
 * @param robot1 A valid robot structure.
 */
void print_robot_xy(robot_t robot1);

/**
 * Moves a robot in a specified direction\n
 * Will print error if direction is invalid.
 * @param robot1 Robot structure that should move
 * @param warehouse Warehouse in which the robot exists
 * @param rows Number of rows in the warehouse
 * @param columns Number of columns in the warehouse
 * @param direction Direction of movement
 */
void move_robot(robot_t* robot1, int* warehouse, int rows, int columns, direction_e direction);

/**
 * A test function for manually moving a robot in a warehouse.
 * @param robot1 Robot structure that should move
 * @param warehouse Warehouse in which the robot exists
 * @param rows Number of rows in the warehouse
 * @param columns Number of columns in the warehouse
 * @param shelves Pointer to array of shelves structs
 * @param n_shelves Number of shelves
 */
void manual_movement(robot_t* robot1, int* warehouse, int rows, int columns, shelf_t* shelves[], int n_shelves, item_t pickingItems[]);

/**
 * A function that checks for nearby shelves y+1 / y-1 from the robot
 * @param robot Pointer to the robot struct
 * @param shelves Pointer to array of shelves structs
 * @param n_shelves Number of shelves
 */
void check_nearby_shelves(robot_t* robot, shelf_t* shelves[], int n_shelves, item_t pickingItems[]);

/**
 * A function that can pickup items from a shelf
 * @param robot
 * @param shelf
 * @param amount
 */
void robot_item_pickup(robot_t* robot, shelf_t* shelf, int amount);


/**
 * A function that empties the robot's inventory and returns the amount of items dropped as an integer
 * @param robot Pointer to the robot struct
 * @param drop_zones Array with the drop zones
 * @param drop_zone_amount Number of drop zones
 */
int robot_drop_all(robot_t* robot, drop_zones* drop_zones);

/**
 * A function that returns 1 if the robot is in a valid drop zone
 * @param robot Pointer to the robot struct
 * @param drop_zones Array with the drop zones
 * @param drop_zone_amount Number of drop zones
 */
int is_robot_in_drop_zone(robot_t* robot, drop_zones* drop_zones);

/**
 * A function that frees the robot memory allocation
 * @param robot1 Pointer to the struct robot_t that is being freed
 */
void free_robot(robot_t* robot1);