#include "robot.h"
#include "mtest.h"
#include "warehouse.h"

TEST_CASE(robot_moves_north,
/*--------------- Arrange ---------------*/
    const direction_e direction = north;

    const int robot_id = 1;
    const int n_items = 0;
    const int x = 1;
    const int y = 1;

    const int columns = 3;
    const int rows = 3;

    int warehouse[] = {
        empty, empty, empty,
        empty, robot, empty,
        empty, empty, empty,
    };
    robot_t robot = {robot_id, n_items, {0}, x, y};
/*--------------- Act ---------------*/
    move_robot(&robot, warehouse, rows, columns, direction);
/*--------------- Assert ---------------*/
    CHECK_EQ_INT(robot.x, x);
    CHECK_EQ_INT(robot.y, y-1);
)


TEST_CASE(robot_moves_south,
/*--------------- Arrange ---------------*/
    const direction_e direction = south;

    const int robot_id = 1;
    const int n_items = 0;
    const int x = 1;
    const int y = 1;

    const int columns = 3;
    const int rows = 3;

    int warehouse[] = {
        empty, empty, empty,
        empty, robot, empty,
        empty, empty, empty,
    };

    robot_t robot = {robot_id, n_items, {0}, x, y};
/*--------------- Act ---------------*/
    move_robot(&robot, warehouse, rows, columns, direction);
/*--------------- Assert ---------------*/
    CHECK_EQ_INT(robot.x, x);
    CHECK_EQ_INT(robot.y, y+1);
)


TEST_CASE(robot_moves_west,
/*--------------- Arrange ---------------*/
    const direction_e direction = west;

    const int robot_id = 1;
    const int n_items = 0;
    const int x = 1;
    const int y = 1;

    const int columns = 3;
    const int rows = 3;

    int warehouse[] = {
        empty, empty, empty,
        empty, robot, empty,
        empty, empty, empty,
    };

    robot_t robot = {robot_id, n_items, {0}, x, y};
/*--------------- Act ---------------*/
    move_robot(&robot, warehouse, rows, columns, direction);
/*--------------- Assert ---------------*/
    CHECK_EQ_INT(robot.x, x-1);
    CHECK_EQ_INT(robot.y, y);
)


TEST_CASE(robot_moves_east,
/*--------------- Arrange ---------------*/
    const direction_e direction = east;

    const int robot_id = 1;
    const int n_items = 0;
    const int x = 1;
    const int y = 1;

    const int columns = 3;
    const int rows = 3;

    int warehouse[] = {
        empty, empty, empty,
        empty, robot, empty,
        empty, empty, empty,
    };

    robot_t robot = {robot_id, n_items, {0}, x, y};
/*--------------- Act ---------------*/
    move_robot(&robot, warehouse, rows, columns, direction);
/*--------------- Assert ---------------*/
    CHECK_EQ_INT(robot.x, x+1);
    CHECK_EQ_INT(robot.y, y);
)


TEST_CASE(move_robot_updates_arena,
/*--------------- Arrange ---------------*/
    const direction_e direction = east;

    const int robot_id = 1;
    const int n_items = 0;
    const int x = 1;
    const int y = 1;

    const int columns = 3;
    const int rows = 3;

    int warehouse[] = {
        empty, empty, empty,
        empty, robot, empty,
        empty, empty, empty,
    };

    robot_t robot1 = {robot_id, n_items, {0}, x, y};
/*--------------- Act ---------------*/
    move_robot(&robot1, warehouse, rows, columns, direction);
/*--------------- Assert ---------------*/
    for (int i = 0; i < columns; i++) {
        for (int j = 0; j < rows; j++) {
            if (i == x + 1 && j == y) {
                CHECK_EQ_INT(warehouse[j*rows+i], robot);
            } else {
                CHECK_EQ_INT(warehouse[j*rows+i], empty);
            }
        }
    }
)


MAIN_RUN_TESTS(robot_moves_north, robot_moves_south, robot_moves_east, robot_moves_west, move_robot_updates_arena)