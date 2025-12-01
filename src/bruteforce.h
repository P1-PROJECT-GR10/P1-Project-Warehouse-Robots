#include "robot.h"
#include "math.h"
#include "warehouse.h"
#include <stdbool.h>

/** Struct for the neighbours of the robot.
 *  There are four used in the code, one for each cardinal direction.
 *  @distance   -   The int distance stores the neighbours absolute manhattan distance to the goal point.
 *  @direction  -   The direction that the neighbour is facing away from the robots current position.
 *                  These are: North, South, East and west.
 *                  We have decided, at least for now, to only allow the robot to move in these for directions,
 *                  and not in between them, for ease of implementation.
 */
typedef struct neighbour {
    int distance;
    direction_e direction;
    int x;
    int y;
    cell_e cell;
} neighbour_t;

int manhat_dist(int x, int y, int goal_x, int goal_y);

/** The main function for the bruteforce algorithm, here lies the conditions for the function.
 *  It also prints the total number of steps (amount of recursive calls) it uses.
 * @param warehouse -   The warehouse layout.
 * @param robot     -   The robot that is moving
 * @param goal_x    -   The x-coordinate of the goal point for the algorithm.
 * @param goal_y    -   The y-coordinate of the goal point for the algorithm.
 * @param columns   -   The paramater for the columns, and also the rows, of the warehouse.
 * @param rows      -   They are unnessecary, and will be removed when cleaned up.
 */
void bruteforce(int* warehouse, robot_t* robot, int goal_x, int goal_y, int columns, int rows);

/** The recoursive part of the bruteforcing algorithm.
 *  Moves the robot one step at a time until it has gotten to its goal point.
 * @param warehouse -   The warehouse layout.
 * @param robot     -   The robot that is moving
 * @param goal_x    -   The x-coordinate of the goal point for the algorithm.
 * @param goal_y    -   The y-coordinate of the goal point for the algorithm.
 * @param columns   -   The paramater for the columns, and also the rows, of the warehouse.
 * @param rows          They are unnessecary, and will be removed when cleaned up.
 * @param prev      -   The previous direction that has been moved by the robot.
 *                      This is used to stop the robot from moving back and fourth,
 *                      by making it unable to move back the way that is just came.
 * @param moves     -   The amount of moves that the robot has made.
 *                  -   Used to keep track of the algorithms efficiency.
 * @return
 */
int bruteforce_recursive(int* warehouse, robot_t* robot, int goal_x, int goal_y, int columns, int rows, direction_e prev, int moves);

bool is_in_bounds(int x, int y, int rows, int columns);