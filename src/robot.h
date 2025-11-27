#define ROBOT_MAX_CAPACITY 8
typedef enum direction {north, south, east, west} direction_e;

typedef struct { // the structure for the robot(s)
    int robot_id; // the id of the robot
    //  double carried_weight;
    int number_of_items;
    item_t item[ROBOT_MAX_CAPACITY];
    //item_t item[8] // noget alla dette for items på roboten, kræver adgang til item branch
    // this could be changed to maybe a dynamic amount latter possibly.
    int x; // the coordinates of the robot, stored as separate integers
    int y;
} robot_t;

robot_t* create_robot();
void print_robot1_xy(robot_t robot1);
void move_robot(robot_t* robot1, int* warehouse, int rows, int columns, direction_e direction);
void manual_movement(robot_t* robot1, int* warehouse, int rows, int columns, shelf_t* shelves[], int n_shelves);
void check_nearby_shelves(robot_t* robot, shelf_t* shelves[], int n_shelves);
void robot_item_pickup(robot_t* robot, shelf_t* shelf, int amount);
