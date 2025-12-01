#include "a_star.h"

int manhattan_h(node_t current, node_t goal) {
    return abs(current.x - goal.x) + abs(current.y - goal.y);
}

/*
 *#######################################################################
 *##                                                                   ##
 *##                         MinHeap Functions                         ##
 *##                                                                   ##
 *#######################################################################
 */


minheap* create_minheap(int capacity) {
    minheap* heap = (minheap*)malloc(sizeof(minheap));
    heap->size = 0;
    heap->capacity = capacity;
    heap->array = (node_t**)malloc(sizeof(node_t*) * capacity);
    return heap;
}

void heap_swap(minheap* heap, int i, int j) {
    // Swap node pointers
    node_t* temp = heap->array[i];
    heap->array[i] = heap->array[j];
    heap->array[j] = temp;

    // Update heap_index of nodes
    heap->array[i]->heap_index = i;
    heap->array[j]->heap_index = j;
}

// Fixes heap from index to 0
void heapify_up(minheap* heap, int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        // If child f-cost is lesser than parent: swap
        if (heap->array[index]->f < heap->array[parent]->f) {
            // Swap child and parent
            heap_swap(heap, index, parent);
        } else {
            break;
        }
        // Set index to parent index and repeat
        index = parent;
    }
}

void heapify_down(minheap* heap, int index) {
    int smallest = index;           // Assume the index is the smallest
    int left = index * 2 + 1;       // Index of left child
    int right = index * 2 + 2;      // Index of right child

    // Check left child
    if (left < heap->size && heap->array[left]->f < heap->array[smallest]->f) {
        smallest = left;
    }

    // Check right child
    if (right < heap->size && heap->array[right]->f < heap->array[smallest]->f) {
        smallest = right;
    }

    // Check whether to swap
    if (smallest != index) {
        // Swap with smallest f-cost
        heap_swap(heap, index, smallest);

        // Recursively heapify next "tree"
        heapify_down(heap, smallest);
    }
}

void heap_push(minheap* heap, node_t* np) {
    // Error handling
    if (heap->size == heap->capacity) {
        printf("Heap is full!");
        return;
    }

    int i = heap->size++;      // Get index for storing node pointer and increase size
    heap->array[i] = np;       // Set node pointer at index
    np->heap_index = i;     // Tell node where it is
    heapify_up(heap, i);       // Fix heap
}

node_t* heap_pop(minheap* heap) {
    // Error handling
    if (heap->size == 0) {
        printf("Heap is empty!");
        return NULL;
    }

    node_t* root = heap->array[0];
    root->heap_index = -1;      // No longer in heap
    heap->size--;                  // Heap is smaller

    // Move last node to the top
    heap->array[0] = heap->array[heap->size];    // Doesnt use heap_swap for efficiency
    if (heap->size > 0) {                  // If popped element was not the last element
        heap->array[0]->heap_index = 0;
        heapify_down(heap, 0);
    }

    return root;
}

/*
 *#######################################################################
 *##                                                                   ##
 *##                           A*  Functions                           ##
 *##                                                                   ##
 *#######################################################################
 */

int get_index(int x, int y, int columns) {
    return y * columns + x;
}

bool is_in_bounds(int x, int y, int rows, int columns) {
    if (x >= 0 && x < columns && y >= 0 && y < rows) {
        return true;
    } else {
        return false;
    }
}

node_t* create_node_map(int rows, int columns) {
    node_t* node_map = malloc(sizeof(node_t) * rows * columns);
    return node_map;
}

void reset_node_map(node_t* node_map, int* warehouse, int rows, int columns) {
    int total_size = rows * columns;

    for (int i = 0; i < total_size; i++) {
        // Calculate x and y values
        node_map[i].x = i % columns;
        node_map[i].y = i / columns;

        // Reset A* values
        node_map[i].g = INFINITY;
        node_map[i].f = INFINITY;
        node_map[i].h = 0;
        node_map[i].parent = NULL;
        node_map[i].visited = false;
        node_map[i].heap_index = -1;

        // Read from warehouse if nodes are obstacles
        if (warehouse[i] == shelf) {
            node_map[i].obstacle = true;
        } else {
            node_map[i].obstacle = false;
        }
    }
}

node_t* a_star(int* warehouse, node_t* node_map, int rows, int columns, int start_x, int start_y, int goal_x, int goal_y) {

    // Reset the node map
    reset_node_map(node_map, warehouse, rows, columns);

    // Setup start and goal points
    int start_index = get_index(start_x, start_y, columns);
    int goal_index  = get_index(goal_x, goal_y, columns);

    node_t* start_node = &node_map[start_index];
    node_t* goal_node = &node_map[goal_index];

    // Error handling for invalid start or goal
    if (start_node->obstacle || goal_node->obstacle) {
        printf("Invalid start or goal point");
        return NULL;
    }

    // Create MinHeap
    minheap* open_set = create_minheap(rows * columns);

    start_node->g = 0;
    start_node->h = manhattan_h(*start_node, *goal_node);
    start_node->f = start_node->g + start_node->h;

    heap_push(open_set, start_node);

    // Main loop
    while (open_set->size > 0) {
        // Pop current node (node with lowest f-cost) from open_set
        node_t* current = heap_pop(open_set);
        current->visited = true;

        // Check if goal is reached
        if (current->x == goal_x && current->y == goal_y) {
            // Clean up and return
            free(open_set->array);
            free(open_set);
            return current;
        }

        // Check neighbours
        int directions[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

        for (int i = 0; i < 4; i++) {
            // Set up neighbours coords
            int nx = current->x + directions[i][0];
            int ny = current->y + directions[i][1];

            // Check is neighbour is inside warehouse
            if (is_in_bounds(nx, ny, rows, columns)) {
                // Get neighbour node from map
                int n_index = get_index(nx, ny, columns);
                node_t* neighbour = &node_map[n_index];

                // Check if visited or obstacle
                if (neighbour->obstacle || neighbour->visited) continue;

                // Calculate g-cost (cost pr. step is 1.0)
                float tentative_g = current->g + 1;

                // Check if better path was found
                if (tentative_g < neighbour->g) {
                    // Set up neighbour values
                    neighbour->parent = current;
                    neighbour->g = tentative_g;
                    neighbour->h = manhattan_h(*neighbour, *goal_node);
                    neighbour->f = neighbour->g + neighbour->h;

                    // Push neighbour to open_set or reorder heap with new values
                    if (neighbour->heap_index == -1) {
                        heap_push(open_set, neighbour);
                    } else {
                        heapify_up(open_set, neighbour->heap_index);
                    }
                }
            }
        }
    }
    // No path found
    printf("No path was found :(");
    free(open_set->array);
    free(open_set);
    return NULL;
}

direction_e* reconstruct_path(node_t* goal_node, int* path_length) {
    if (goal_node == NULL) {
        *path_length = 0;
        return NULL;
    }

    // Since g-cost is 1 pr step we assume the length of the path is the goal node's g-cost
    int steps = (int)goal_node->g;
    *path_length = steps;

    // Allocate memory for path
    direction_e* path = (direction_e*)malloc(sizeof(direction_e)*steps);

    node_t* current = goal_node;

    // Fill the path "from behind"
    int index = steps - 1;

    while (current->parent != NULL) {
        node_t* parent = current->parent;

        int dx = current->x - parent->x;
        int dy = current->y - parent->y;

        // Decide direction
        if (dx == 1) {
            path[index] = east;
        } else if (dx == -1) {
            path[index] = west;
        } else if (dy == 1) {
            path[index] = south;
        } else if (dy == -1) {
            path[index] = north;
        }

        // Step back in path
        current = parent;
        index--;
    }
    return path;
}

void move_robot_to_point(robot_t* robot, int* warehouse, int rows, int columns, int goal_x, int goal_y) {
    // Create node map for A* algorithm
    node_t* node_map = create_node_map(rows, columns);

    // Find path to point
    node_t* result = a_star(warehouse, node_map, rows, columns, robot->x, robot->y, goal_x, goal_y);

    if (result != NULL) {
        printf("\nPath found!\n");
        int length = 0;

        // Reconstruct Path
        direction_e* path = reconstruct_path(result, &length);

        printf("Path length is: %d\n\n", length);

        // Print current robot position
        printf("Robot starts at x: %d, y: %d\n", robot->x, robot->y);
        print_warehouse(warehouse, rows, columns);
        printf("\n");

        // Move robot
        for (int i = 0; i< length; i++) {
            move_robot(robot, warehouse, rows, columns, path[i]);
            printf("Robot moves %s to x: %d, y: %d\n", direction_to_string(path[i]), robot->x, robot->y);
            //print_warehouse(warehouse, rows, columns);
            //printf("\n"); // For readability

        }
        free(path);
    } else {
        printf("\nNo path found\n");
    }
    print_node_map(node_map, rows, columns);
    free(node_map);
}

void robot_get_picking_list(robot_t* robot1, int* warehouse, int rows, int columns, item_t* picking_list, int amount_of_picking_items, shelf_t** shelves, int n_shelves) {
    for (int i = 0; i < amount_of_picking_items; i++) {

        shelf_t* goal_shelf = search_item(picking_list[i].color, picking_list[i].name, shelves, n_shelves);


        int goal_x = goal_shelf->x;
        int goal_y;

        int index = get_index(goal_shelf->x, goal_shelf->y+1, columns);
        if (warehouse[index] == empty) {
            goal_y = goal_shelf->y + 1;
        } else {
            goal_y = goal_shelf->y - 1;
        }

        printf("Item %d found at shelf x: %d, y: %d\n"
               "Navigating to (%d, %d)\n", i+1, goal_shelf->x, goal_shelf->y, goal_x, goal_y);

        move_robot_to_point(robot1, warehouse, rows, columns, goal_x, goal_y);
        printf("Robot picks up item %d\n\n", i+1);
    }

    move_robot_to_point(robot1, warehouse, rows, columns, 9, 9); // Move robot back to (9, 9) or a dropzone
}

direction_e parent_direction(node_t node) {

    if (node.parent == NULL) return no_direction;

    int dx = node.x - node.parent->x;
    int dy = node.y - node.parent->y;

    if (dx == -1) return west;
    if (dx == 1) return east;
    if (dy == -1) return north;
    if (dy == 1) return south;

    return EXIT_FAILURE;
}

char* node_came_from_to_string(node_t node) {
    if (node.obstacle) return "|X";

    if (node.visited) {
        direction_e camefrom = parent_direction(node);
        switch (camefrom) {
            case north:
                return "|^";
            case south:
                return "|v";
            case east:
                return "|>";
            case west:
                return "|<";
            case no_direction:
                return "|G";
        }
    }
    return "| ";
}

void print_node_map(node_t* node_map, int rows, int columns) {
    // Print row of x-coords
    printf("\nY: X:");
    for (int x = 0; x < columns; x++) {
        printf("%d ", x % 10);
    }
    printf("\n");

    for (int y = 0; y < rows; y++) {
        printf("%d - ", y % 10);    // Prints y-coords
        for (int x = 0; x < columns; x++) {
            int index = get_index(x, y, columns);
            printf("%s", node_came_from_to_string(node_map[index]));
        }
        printf("|\n");
    }
}
