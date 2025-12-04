#include "robot.h"
#include "mtest.h"
#include "warehouse.h"

TEST_CASE(robot_moves_north,
/*--------------- Arrange ---------------*/
    const direction_e direction = north;

    warehouse_t* warehouse = create_warehouse();

    const int n_items = 0;
    const int n_steps = 0;
    const int x = 1;
    const int y = 1;

    robot_t robot = {n_items, {0}, x, y, n_steps};
    /*--------------- Act ---------------*/
    move_robot(&robot, warehouse, direction);
    /*--------------- Assert ---------------*/
    CHECK_EQ_INT(robot.x, x);
    CHECK_EQ_INT(robot.y, y-1);

    destroy_warehouse(warehouse);
)

TEST_CASE(robot_moves_south,
/*--------------- Arrange ---------------*/
    const direction_e direction = south;

    warehouse_t* warehouse = create_warehouse();

    const int n_items = 0;
    const int n_steps = 0;
    const int x = 1;
    const int y = 1;

    robot_t robot = {n_items, {0}, x, y, n_steps};
    /*--------------- Act ---------------*/
    move_robot(&robot, warehouse, direction);
    /*--------------- Assert ---------------*/
    CHECK_EQ_INT(robot.x, x);
    CHECK_EQ_INT(robot.y, y+1);

    destroy_warehouse(warehouse);
)

TEST_CASE(robot_moves_west,
/*--------------- Arrange ---------------*/
    const direction_e direction = west;

    warehouse_t* warehouse = create_warehouse();

    const int n_items = 0;
    const int n_steps = 0;
    const int x = 1;
    const int y = 1;

    robot_t robot = {n_items, {0}, x, y, n_steps};
    /*--------------- Act ---------------*/
    move_robot(&robot, warehouse, direction);
    /*--------------- Assert ---------------*/
    CHECK_EQ_INT(robot.x, x-1);
    CHECK_EQ_INT(robot.y, y);

    destroy_warehouse(warehouse);
)

TEST_CASE(robot_moves_east,
/*--------------- Arrange ---------------*/
    const direction_e direction = east;

    warehouse_t* warehouse = create_warehouse();

    const int n_items = 0;
    const int n_steps = 0;
    const int x = 1;
    const int y = 1;

    robot_t robot = {n_items, {0}, x, y, n_steps};
    /*--------------- Act ---------------*/
    move_robot(&robot, warehouse, direction);
    /*--------------- Assert ---------------*/
    CHECK_EQ_INT(robot.x, x); // Robot drives into shelf, so shouldn't move
    CHECK_EQ_INT(robot.y, y);

    destroy_warehouse(warehouse);
)

TEST_CASE(move_robot_updates_arena,
/*--------------- Arrange ---------------*/
    const direction_e direction = north;

    warehouse_t* warehouse = create_warehouse();

    const int n_items = 0;
    const int n_steps = 0;
    const int x = 1;
    const int y = 1;

    robot_t test_robot = {n_items, {0}, x, y, n_steps};
    /*--------------- Act ---------------*/
    move_robot(&test_robot, warehouse, direction);
    /*--------------- Assert ---------------*/
    CHECK_EQ_INT(warehouse->map[0*warehouse->rows+1], robot);
    CHECK_EQ_INT(warehouse->map[1*warehouse->rows+1], empty);

    destroy_warehouse(warehouse);
)


MAIN_RUN_TESTS(robot_moves_north, robot_moves_south, robot_moves_east, robot_moves_west, move_robot_updates_arena)