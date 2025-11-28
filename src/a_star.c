#include "a_star.h"

#include <stdio.h>
#include <stdlib.h>

int manhattan_h(node_t current, int goal_x, int goal_y) {
    return abs(current.x - goal_x) + abs(current.y - goal_y);
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
        if (h->array[index]->f > h->array[parent]->f) {
            // Swap child and parent
            heap_swap(h, index, parent);
        } else {
            break;;
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
    root->heap_index-1;     // No longer in heap
    h->size--;              // Heap is smaller

    // Move last node to the top
    h->array[0] = h->array[h->size];    // Doesnt use heap_swap for efficiency
    if (h->size > 0) {                  // If popped element was not the last element
        h->array[0]->heap_index = 0;
        heapify_down(h, 0);
    }

    return root;
}