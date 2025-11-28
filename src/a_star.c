#include "a_star.h"

#include <math.h>
#include "warehouse.h"

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
    minheap* h = (minheap*)malloc(sizeof(minheap));
    h->size = 0;
    h->capacity = capacity;
    h->array = (node_t**)malloc(sizeof(node_t*) * capacity);
    return h;
}

void heap_swap(minheap* h, int i, int j) {
    // Swap node pointers
    node_t* temp = h->array[i];
    h->array[i] = h->array[j];
    h->array[j] = temp;

    // Update heap_index of nodes
    h->array[i]->heap_index = i;
    h->array[j]->heap_index = j;
}

// Fixes heap from index to 0
void heapify_up(minheap* h, int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        // If child f-cost is lesser than parent: swap
        if (h->array[index]->f < h->array[parent]->f) {
            // Swap child and parent
            heap_swap(h, index, parent);
        } else {
            break;
        }
        // Set index to parent index and repeat
        index = parent;
    }
}

void heapify_down(minheap* h, int index) {
    int smallest = index;           // Assume the index is the smallest
    int left = index * 2 + 1;       // Index of left child
    int right = index * 2 + 2;      // Index of right child

    // Check left child
    if (left < h->size && h->array[left]->f < h->array[smallest]->f) {
        smallest = left;
    }

    // Check right child
    if (right < h->size && h->array[right]->f < h->array[smallest]->f) {
        smallest = right;
    }

    // Check whether to swap
    if (smallest != index) {
        // Swap with smallest f-cost
        heap_swap(h, index, smallest);

        // Recursively heapify next "tree"
        heapify_down(h, smallest);
    }
}

void heap_push(minheap* h, node_t* np) {
    // Error handling
    if (h->size == h->capacity) {
        printf("Heap is full!");
        return;
    }

    int i = h->size++;      // Get index for storing node pointer and increase size
    h->array[i] = np;       // Set node pointer at index
    np->heap_index = i;     // Tell node where it is
    heapify_up(h, i);       // Fix heap
}

node_t* heap_pop(minheap* h) {
    // Error handling
    if (h->size == 0) {
        printf("Heap is empty!");
        return NULL;
    }

    node_t* root = h->array[0];
    root->heap_index = -1;     // No longer in heap
    h->size--;              // Heap is smaller

    // Move last node to the top
    h->array[0] = h->array[h->size];    // Doesnt use heap_swap for efficiency
    if (h->size > 0) {                  // If popped element was not the last element
        h->array[0]->heap_index = 0;
        heapify_down(h, 0);
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

bool is_valid(int x, int y, int rows, int columns) {
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
            if (is_valid(nx, ny, rows, columns)) {
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
// Damn that was stressful