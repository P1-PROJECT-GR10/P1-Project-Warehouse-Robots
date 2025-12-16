#pragma once
#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "robot.h"
#include "warehouse.h"


//---------------------------------------STRUCTURES---------------------------------------

/**
 * A structure for a map-node used in the A* pathfinding algorithm with relevant variables,
 */
typedef struct node {
    int x, y;               ///< coordinate for node position
    int g, h ,f;          ///< A* related cost variables
    bool visited;           ///< Whether node has already been visited
    bool obstacle;          ///< Whether node is an obstacle
    struct node* parent;    ///< Back pointer for path reconstruction

    int heap_index;         ///< Position in heap, -1 if not in heap
} node_t;

/**
 * A structure for a minheap containing the size, capacity, and an array of node pointers.
 */
typedef struct {
    node_t** array;         ///< An array of node pointers
    int size;               ///< The current size of the MinHeap
    int capacity;           ///< The maximum capacity of the MinHeap
} minheap;

//---------------------------------------FUNCTIONS---------------------------------------

/**
 * Creates a heap allocated MinHeap
 * @attention Remember to free allocated memory!
 * @param capacity The maximum capacity of the MinHeap
 * @return A pointer to a heap allocated MinHeap
 */
minheap* create_minheap(int capacity);

/**
 * Swaps two nodes in the MinHeap
 * @param heap A valid MinHeap
 * @param i Index of first node
 * @param j Index of second node
 */
void heap_swap(minheap* heap, int i, int j);

/**
 * Function for reordering MinHeap from a node and up through its parents by "bubbling up"
 * @param heap A valid MinHeap
 * @param index Index of the starting node
 */
void heapify_up(minheap* heap, int index);

/**
 * Recursively reordering MinHeap from a root and down through its children
 * @param heap A valid MinHeap
 * @param index Index of the starting root
 */
void heapify_down(minheap* heap, int index);

/**
 * Pushes a node to the MinHeap
 * @param heap A valid MinHeap
 * @param np A pointer to the node that will be pushed
 */
void heap_push(minheap* heap, node_t* np);

/**
 * Deletes the root of the MinHeap aka the node with the lowest f-cost in the tree and returns a pointer to said node
 * @param heap A valid MinHeap
 * @return A pointer to the former root of the MinHeap
 */
node_t* heap_pop(minheap* heap);

/**
 * Helper function for getting the index of a point in a 1D array
 * @param x x-coordinate for desired point
 * @param y y-coordinate for desired point
 * @param columns The amount of columns in the grid or the width
 * @return the index corresponding to the input point
 */
int get_index(int x, int y, int columns);

/**
 * Creates a heap allocated 1D grid array of node structs
 * @attention Remember to free allocated memory!
 * @param warehouse The warehouse heap
 * @return A pointer to an array of structs in a grid
 */
node_t* create_node_map(const warehouse_t* warehouse);

/**
 * Resets a map of nodes according to a map of a warehouse.
 * Node and warehouse map should have same amount of rows and columns
 * @param node_map Map of nodes
 * @param warehouse Map of warehouse
 */
void reset_node_map(node_t* node_map, const warehouse_t* warehouse);

/**
 * A pathfinding algorithm for finding the shortest path from a start point to a goal point
 * @param warehouse Map of a warehouse
 * @param node_map Map of nodes
 * @param start_x x-coordinate of the start point
 * @param start_y y-coordinate of the start point
 * @param goal_x x-coordinate of the goal point
 * @param goal_y y-coordinate of the goal point
 * @return a pointer to the goal node with the appropriate back pointers for reconstructing the optimal path
 */
node_t* a_star(const warehouse_t* warehouse, node_t* node_map, int start_x, int start_y, int goal_x, int goal_y);

/**
 * Reconstructs a path from a goal node through identifying parent nodes
 * @param goal_node The resulting goal_node from an A* pathfinding algorithm
 * @param path_length Total length of the path (usually the g-cost)
 * @return An array of directions indicating as a "move list" resulting in the correct path
 */
direction_e* reconstruct_path(node_t* goal_node, int* path_length);

/**
 * A function that uses A* pathfinding and path reconstruction for moving a robot to a point
 * @param robot The robot that should move
 * @param warehouse An array representing the map of a warehouse
 * @param goal_x x-coordinate of the goal point
 * @param goal_y y-coordinate of the goal point
 */
void move_robot_to_point(robot_t* robot, const warehouse_t* warehouse, int goal_x, int goal_y);

/**
 * A function that moves the robot to the points from where it can pick up items in a picking list
 * @param robot1 A pointer to a valid robot structure
 * @param warehouse The warehouse heap
 * @param picking_list An array of items for the robot to pickup
 */
void robot_get_picking_list(robot_t* robot1, const warehouse_t* warehouse, picking_list_t* picking_list);

/**
 * Function for checking which direction a node came from
 * @param node A valid node type
 * @return the direction from the node to the parent
 */
direction_e parent_direction(node_t node);

/**
 * For printing an arrowhead indicating where a nodes back pointer is pointing
 * @param node A valid node type
 * @return a string representing a cell with a pointer
 */
char* node_came_from_to_string(node_t node);

/**
 * Prints a map of nodes evaluated by the A* algorithm and their back pointers
 * @param node_map A map of A* nodes
 * @param rows The amount of rows in the map
 * @param columns The amount of columns in the map.
 */
void print_node_map(node_t* node_map, int rows, int columns, int goal_index);

/**
 * Return whether a point is inside the width and height of a grid
 * @param x x-coordinate of a point
 * @param y y-coordinate of a point
 * @param warehouse The warehouse heap
 * @return a boolean value representing whether the point is in bounds
 */
bool is_in_bounds(int x, int y, const warehouse_t* warehouse);