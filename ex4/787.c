#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INF 1e9

int findCheapestPrice(int n, int** flights, int flightsSize, int* flightsColSize, int src, int dst, int k) {
    // Distance array to store the minimum cost to reach each city
    int* dist = (int*)malloc(n * sizeof(int));
    // Temporary array to store distances from the previous iteration
    int* temp = (int*)malloc(n * sizeof(int));
    
    // Initialize all distances to infinity
    for (int i = 0; i < n; i++) {
        dist[i] = INF;
    }
    dist[src] = 0; // Cost to reach source is 0

    // Run relaxation up to k + 1 times (at most k stops means at most k + 1 flights)
    for (int i = 0; i <= k; i++) {
        // Copy current distances to temp array
        memcpy(temp, dist, n * sizeof(int));
        
        // Iterate through all flights
        for (int j = 0; j < flightsSize; j++) {
            int u = flights[j][0];
            int v = flights[j][1];
            int price = flights[j][2];
            
            // If the starting city of the flight is reachable
            if (dist[u] != INF) {
                if (dist[u] + price < temp[v]) {
                    temp[v] = dist[u] + price;
                }
            }
        }
        // Update main distance array for the next level/stop iteration
        memcpy(dist, temp, n * sizeof(int));
    }

    int result = dist[dst];
    
    // Free allocated memory
    free(dist);
    free(temp);

    // If destination is unreachable within k stops, return -1
    return (result == INF) ? -1 : result;
}
