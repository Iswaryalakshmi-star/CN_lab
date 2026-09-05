#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define INF 1e9

// Structure to represent an edge in the adjacency list
typedef struct Node {
    int target;
    int weight;
    struct Node* next;
} Node;

// Structure to represent an element in the min-heap
typedef struct {
    int node;
    int dist;
} HeapNode;

// Structure for the Min-Heap
typedef struct {
    HeapNode* data;
    int size;
    int capacity;
} MinHeap;

// Min-Heap Helper Functions
MinHeap* createMinHeap(int capacity) {
    MinHeap* heap = (MinHeap*)malloc(sizeof(MinHeap));
    heap->data = (HeapNode*)malloc(capacity * sizeof(HeapNode));
    heap->size = 0;
    heap->capacity = capacity;
    return heap;
}

void swap(HeapNode* a, HeapNode* b) {
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

void push(MinHeap* heap, int node, int dist) {
    if (heap->size == heap->capacity) return;
    heap->data[heap->size].node = node;
    heap->data[heap->size].dist = dist;
    int i = heap->size;
    heap->size++;
    
    while (i > 0 && heap->data[i].dist < heap->data[(i - 1) / 2].dist) {
        swap(&heap->data[i], &heap->data[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

HeapNode pop(MinHeap* heap) {
    HeapNode top = heap->data[0];
    heap->size--;
    heap->data[0] = heap->data[heap->size];
    
    int i = 0;
    while (2 * i + 1 < heap->size) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int smallest = left;
        
        if (right < heap->size && heap->data[right].dist < heap->data[left].dist) {
            smallest = right;
        }
        if (heap->data[i].dist <= heap->data[smallest].dist) break;
        
        swap(&heap->data[i], &heap->data[smallest]);
        i = smallest;
    }
    return top;
}

// Main Function to calculate network delay time
int networkDelayTime(int** times, int timesSize, int* timesColSize, int n, int k) {
    // 1. Build the Adjacency List (1-indexed nodes)
    Node** adj = (Node**)calloc((n + 1), sizeof(Node*));
    for (int i = 0; i < timesSize; i++) {
        int u = times[i][0];
        int v = times[i][1];
        int w = times[i][2];
        
        Node* newNode = (Node*)malloc(sizeof(Node));
        newNode->target = v;
        newNode->weight = w;
        newNode->next = adj[u];
        adj[u] = newNode;
    }
    
    // 2. Initialize Distance Array
    int* dist = (int*)malloc((n + 1) * sizeof(int));
    for (int i = 1; i <= n; i++) {
        dist[i] = INF;
    }
    dist[k] = 0;
    
    // 3. Dijkstra's Algorithm
    MinHeap* heap = createMinHeap(timesSize + 1);
    push(heap, k, 0);
    
    while (heap->size > 0) {
        HeapNode curr = pop(heap);
        int u = curr.node;
        int d = curr.dist;
        
        // Skip stale heap entry
        if (d > dist[u]) continue;
        
        // Traverse all neighbors of u
        Node* neighbor = adj[u];
        while (neighbor != NULL) {
            int v = neighbor->target;
            int weight = neighbor->weight;
            
            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                push(heap, v, dist[v]);
            }
            neighbor = neighbor->next;
        }
    }
    
    // 4. Find the maximum time to reach any node
    int maxTime = 0;
    for (int i = 1; i <= n; i++) {
        if (dist[i] == INF) {
            maxTime = -1; // Unreachable node found
            break;
        }
        if (dist[i] > maxTime) {
            maxTime = dist[i];
        }
    }
    
    // Free allocated memory
    for (int i = 1; i <= n; i++) {
        Node* curr = adj[i];
        while (curr != NULL) {
            Node* temp = curr;
            curr = curr->next;
            free(temp);
        }
    }
    free(adj);
    free(dist);
    free(heap->data);
    free(heap);
    
    return maxTime;
}
