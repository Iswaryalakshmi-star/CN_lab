#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#define INF 1e9

// Structure to represent a cell in the min-heap
typedef struct {
    int row;
    int col;
    int effort;
} Cell;

// Structure for the Min-Heap
typedef struct {
    Cell* data;
    int size;
    int capacity;
} MinHeap;

// Helper functions for Heap operations
MinHeap* createMinHeap(int capacity) {
    MinHeap* heap = (MinHeap*)malloc(sizeof(MinHeap));
    heap->data = (Cell*)malloc(capacity * sizeof(Cell));
    heap->size = 0;
    heap->capacity = capacity;
    return heap;
}

void swap(Cell* a, Cell* b) {
    Cell temp = *a;
    *a = *b;
    *b = temp;
}

void push(MinHeap* heap, Cell cell) {
    if (heap->size == heap->capacity) return;
    heap->data[heap->size] = cell;
    int i = heap->size;
    heap->size++;
    
    while (i > 0 && heap->data[i].effort < heap->data[(i - 1) / 2].effort) {
        swap(&heap->data[i], &heap->data[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

Cell pop(MinHeap* heap) {
    Cell top = heap->data[0];
    heap->size--;
    heap->data[0] = heap->data[heap->size];
    
    int i = 0;
    while (2 * i + 1 < heap->size) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int smallest = left;
        
        if (right < heap->size && heap->data[right].effort < heap->data[left].effort) {
            smallest = right;
        }
        if (heap->data[i].effort <= heap->data[smallest].effort) break;
        
        swap(&heap->data[i], &heap->data[smallest]);
        i = smallest;
    }
    return top;
}

// Main function to find minimum effort
int minimumEffortPath(int** heights, int heightsSize, int* heightsColSize) {
    int rows = heightsSize;
    int cols = heightsColSize[0];
    
    // 2D array to track the minimum effort to reach each cell
    int** effortGrid = (int**)malloc(rows * sizeof(int*));
    for (int i = 0; i < rows; i++) {
        effortGrid[i] = (int*)malloc(cols * sizeof(int));
        for (int j = 0; j < cols; j++) {
            effortGrid[i][j] = INF;
        }
    }
    
    // Min-heap initialization
    MinHeap* heap = createMinHeap(rows * cols);
    
    // Start from top-left cell (0,0) with 0 effort
    effortGrid[0][0] = 0;
    Cell start = {0, 0, 0};
    push(heap, start);
    
    // Direction vectors for moving up, down, left, right
    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};
    
    while (heap->size > 0) {
        Cell curr = pop(heap);
        int r = curr.row;
        int c = curr.col;
        int currEffort = curr.effort;
        
        // If we reached the bottom-right cell, return the effort
        if (r == rows - 1 && c == cols - 1) {
            // Free memory before returning
            for (int i = 0; i < rows; i++) free(effortGrid[i]);
            free(effortGrid);
            free(heap->data);
            free(heap);
            return currEffort;
        }
        
        // If we found a path with higher effort than already recorded, skip it
        if (currEffort > effortGrid[r][c]) continue;
        
        // Explore all 4 neighbors
        for (int i = 0; i < 4; i++) {
            int nr = r + dr[i];
            int nc = c + dc[i];
            
            // Check boundaries
            if (nr >= 0 && nr < rows && nc >= 0 && nc < cols) {
                // Absolute difference in height to the next cell
                int nextEffort = abs(heights[r][c] - heights[nr][nc]);
                // The max effort required for this specific path choice
                int maxEffortOnPath = (currEffort > nextEffort) ? currEffort : nextEffort;
                
                // If this path offers a lower max effort to reach (nr, nc)
                if (maxEffortOnPath < effortGrid[nr][nc]) {
                    effortGrid[nr][nc] = maxEffortOnPath;
                    Cell nextCell = {nr, nc, maxEffortOnPath};
                    push(heap, nextCell);
                }
            }
        }
    }
    
    return 0; // Fallback (should not be reached if bottom-right is accessible)
}
