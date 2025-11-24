#include "a_star.h"

#include <stdlib.h>

int manhatten_h(node_t current, int goal_x, int goal_y) {
    return abs(current.x - goal_x) + abs(current.y - goal_y);
}