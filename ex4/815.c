#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Structure to implement a dynamic array (vector) for stop-to-bus mapping
typedef struct {
    int* data;
    int size;
    int capacity;
} Vector;

void initVector(Vector* v) {
    v->data = NULL;
    v->size = 0;
    v->capacity = 0;
}

void pushVector(Vector* v, int val) {
    if (v->size >= v->capacity) {
        v->capacity = v->capacity == 0 ? 4 : v->capacity * 2;
        v->data = (int*)realloc(v->data, v->capacity * sizeof(int));
    }
    v->data[v->size++] = val;
}

int numBusesToDestination(int** routes, int routesSize, int* routesColSize, int source, int target) {
    if (source == target) return 0;

    // 1. Find the maximum stop number to allocate our mapping array
    int maxStop = 0;
    for (int i = 0; i < routesSize; i++) {
        for (int j = 0; j < routesColSize[i]; j++) {
            if (routes[i][j] > maxStop) {
                maxStop = routes[i][j];
            }
        }
    }

    // If source or target is out of bounds of any existing routes
    if (source > maxStop || target > maxStop) return -1;

    // 2. Build the Stop-to-Buses mapping
    Vector* stopToBuses = (Vector*)calloc((maxStop + 1), sizeof(Vector));
    for (int i = 0; i < routesSize; i++) {
        for (int j = 0; j < routesColSize[i]; j++) {
            int stop = routes[i][j];
            pushVector(&stopToBuses[stop], i);
        }
    }

    // 3. BFS Setup
    bool* visitedBuses = (bool*)calloc(routesSize, sizeof(bool));
    bool* visitedStops = (bool*)calloc((maxStop + 1), sizeof(bool));
    
    // Queue stores pairs: {bus_index, bus_count}
    int* queueBus = (int*)malloc(routesSize * sizeof(int));
    int* queueCount = (int*)malloc(routesSize * sizeof(int));
    int head = 0, tail = 0;

    // Insert all buses that pass through the source stop into the queue
    Vector sourceBuses = stopToBuses[source];
    for (int i = 0; i < sourceBuses.size; i++) {
        int bus = sourceBuses.data[i];
        visitedBuses[bus] = true;
        queueBus[tail] = bus;
        queueCount[tail] = 1; // Taking this bus counts as 1
        tail++;
    }
    visitedStops[source] = true;

    int result = -1;

    // 4. Run BFS
    while (head < tail) {
        int currBus = queueBus[head];
        int busCount = queueCount[head];
        head++;

        // Check all stops this bus can reach
        for (int i = 0; i < routesColSize[currBus]; i++) {
            int stop = routes[currBus][i];

            if (stop == target) {
                result = busCount;
                break;
            }

            if (!visitedStops[stop]) {
                visitedStops[stop] = true;
                
                // Add all unvisited buses that pass through this stop to the queue
                Vector nextBuses = stopToBuses[stop];
                for (int j = 0; j < nextBuses.size; j++) {
                    int nBus = nextBuses.data[j];
                    if (!visitedBuses[nBus]) {
                        visitedBuses[nBus] = true;
                        queueBus[tail] = nBus;
                        queueCount[tail] = busCount + 1;
                        tail++;
                    }
                }
            }
        }
        if (result != -1) break;
    }

    // 5. Clean up Memory
    for (int i = 0; i <= maxStop; i++) {
        if (stopToBuses[i].data != NULL) {
            free(stopToBuses[i].data);
        }
    }
    free(stopToBuses);
    free(visitedBuses);
    free(visitedStops);
    free(queueBus);
    free(queueCount);

    return result;
}
