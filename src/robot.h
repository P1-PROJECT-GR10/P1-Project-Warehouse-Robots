#pragma once


//---------------------------------------ENUMERATIONS---------------------------------------

typedef enum direction {north, south, east, west} direction_e;


//---------------------------------------STRUCTURES---------------------------------------

/**
 * @brief A structure for a robot with relevant variables and parameters.\n
 * Contains the ID, number of items, and x/y-coordinates of the robot.
 */
typedef struct robot_t {
    int robot_id;           ///< The ID of the robot
    int number_of_items;    ///< The number of items the robot is carrying
    int x;                  ///< The x-coordinate of the robot
    int y;                  ///< The y-coordinate of the robot
} robot_t;


//---------------------------------------FUNCTIONS---------------------------------------

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
 */
void manual_movement(robot_t* robot1, int* warehouse, int rows, int columns);