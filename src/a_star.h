#pragma once

typedef struct node {
    int x, y;
    float g, h ,f;
    struct node* parent;
} node_t;

typedef struct point {
    int x, y;
} point_t;

int manhatten_h(node_t current, int goal_x, int goal_y);