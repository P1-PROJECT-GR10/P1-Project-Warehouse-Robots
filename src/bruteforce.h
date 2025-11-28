#include "warehouse.h"
#include "robot.h"
#include "math.h"


// Types
typedef struct neighbour {
    int distance;
    direction_e direction;
    int rowcolumn;
} neighbour_t;

void bruteforce(int* warehouse, robot_t* robot, int x, int y, int columns, int rows, direction_e prev);
void bruteforce_recoursive(int* warehouse, robot_t* robot, int goal_x, int goal_y, int columns, int rows, direction_e prev);