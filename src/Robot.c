//
// Created by ruben on 19-11-2025.
//

/*
Her skal der stå noget om hvordan en robot bliver lavet :).
*/

#include <stdio.h>
#include <stdlib.h>

#include "warehouse.h"

robot_t* create_robot() {

    robot_t* robot1 = (robot_t*)(malloc(sizeof(robot_t)));

    robot1->robot_id = 1;
    robot1->number_of_items = 0;
    robot1->coordinate_x = 9;
    robot1->coordinate_y = 9;

    return robot1;
}

void print_robot1_id(robot_t robot1) { // for testing porpuses
    printf("%d",robot1.robot_id);
}