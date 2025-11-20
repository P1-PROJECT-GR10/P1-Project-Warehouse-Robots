//
// Created by magnu on 14-11-2025.
//

typedef enum cell {empty, shelf, robot} cell_e;
typedef enum row {aisle, shelves} row_e;

int* generate_layout(int main_aisle_width, int aisle_width, int shelf_length, int shelves_amount, int rows, int columns);
void print_cell(cell_e cell);
void print_warehouse(int* warehouse, int rows, int columns);
int* get_cell(int* warehouse, int rows, int columns, int x, int y);

typedef struct { // the structure for the robot(s)
    int robot_id; // the id of the robot
    //  double carried_weight;
    int number_of_items;
    char item[8]; // here I have defined the number of carriable items as a fixed max value,
    // this could be changed to maybe a dynamic amount latter possibly.
    int coordinate_x; // the coordinates of the robot, stored as separate integers
    int coordinate_y;
} robot_t;

robot_t* create_robot();
void print_robot1_id(robot_t robot1);