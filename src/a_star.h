#pragma once
#include <stdbool.h>

typedef struct node {
    int x, y;
    float g, h ,f;
    bool visited;
    bool obstacle;
    struct node* parent;

    int heap_index;     // Position in heap -1 if not in heap
} node_t;

typedef struct {
    node_t** array;
    int size;
    int capacity;
} minheap;


/**
 * A distance function for calculating distance between two points
 * @param current The current node
 * @param goal_x x-coordinate of goal point
 * @param goal_y y-coordinate of goal point
 * @return An integer describing the distance from current node to goal point
 */
int manhattan_h(node_t current, int goal_x, int goal_y);