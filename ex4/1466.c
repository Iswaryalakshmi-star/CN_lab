#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Structure to represent an edge in the adjacency list
typedef struct Edge {
    int to;
    int isOriginal; // 1 if it's the original direction (ai -> bi), 0 if it's reversed (bi -> ai)
    struct Edge* next;
} Edge;

// Main function to find the minimum number of edge changes
int minReorder(int n, int** connections, int connectionsSize, int* connectionsColSize) {
    // 1. Build the Adjacency List
    Edge** adj = (Edge**)calloc(n, sizeof(Edge*));
    for (int i = 0; i < connectionsSize; i++) {
        int u = connections[i][0];
        int v = connections[i][1];
        
        // Add original directed edge: u -> v (takes 1 change if traversed this way away from 0)
        Edge* e1 = (Edge*)malloc(sizeof(Edge));
        e1->to = v;
        e1->isOriginal = 1;
        e1->next = adj[u];
        adj[u] = e1;
        
        // Add artificial reverse edge: v -> u (takes 0 changes if traversed this way)
        Edge* e2 = (Edge*)malloc(sizeof(Edge));
        e2->to = u;
        e2->isOriginal = 0;
        e2->next = adj[v];
        adj[v] = e2;
    }
    
    // 2. BFS Initialization
    bool* visited = (bool*)calloc(n, sizeof(bool));
    int* queue = (int*)malloc(n * sizeof(int));
    int head = 0, tail = 0;
    
    // Start BFS from the capital (city 0)
    queue[tail++] = 0;
    visited[0] = true;
    
    int changeCount = 0;
    
    // 3. Process BFS Traversal
    while (head < tail) {
        int curr = queue[head++];
        
        Edge* edge = adj[curr];
        while (edge != NULL) {
            int nextCity = edge->to;
            
            if (!visited[nextCity]) {
                visited[nextCity] = true;
                // If the edge moves away from the capital in the original direction, we must reverse it
                if (edge->isOriginal == 1) {
                    changeCount++;
                }
                queue[tail++] = nextCity;
            }
            edge = edge->next;
        }
    }
    
    // 4. Free Allocated Memory
    for (int i = 0; i < n; i++) {
        Edge* curr = adj[i];
        while (curr != NULL) {
            Edge* temp = curr;
            curr = curr->next;
            free(temp);
        }
    }
    free(adj);
    free(visited);
    free(queue);
    
    return changeCount;
}
