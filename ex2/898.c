#include <stdlib.h>
#include <stdbool.h>

#define MAX 100000
#define HASH_SIZE 262144

int hashTable[HASH_SIZE];

int hash(int x) {
    return (x * 2654435761u) & (HASH_SIZE - 1);
}

bool exists(int x) {
    int index = hash(x);

    while (hashTable[index] != -1) {
        if (hashTable[index] == x)
            return true;

        index = (index + 1) & (HASH_SIZE - 1);
    }

    return false;
}

void insert(int x) {
    int index = hash(x);

    while (hashTable[index] != -1) {
        if (hashTable[index] == x)
            return;

        index = (index + 1) & (HASH_SIZE - 1);
    }

    hashTable[index] = x;
}

int subarrayBitwiseORs(int* arr, int arrSize) {
    for (int i = 0; i < HASH_SIZE; i++)
        hashTable[i] = -1;

    int prev[32];
    int prevSize = 0;

    int curr[32];
    int currSize;

    int answer = 0;

    for (int i = 0; i < arrSize; i++) {
        currSize = 0;

        // Subarray containing only arr[i]
        curr[currSize++] = arr[i];

        // Extend previous subarrays
        for (int j = 0; j < prevSize; j++) {
            int value = prev[j] | arr[i];

            // Avoid duplicate consecutive OR values
            if (curr[currSize - 1] != value) {
                curr[currSize++] = value;
            }
        }

        // Add current OR values to the global set
        for (int j = 0; j < currSize; j++) {
            if (!exists(curr[j])) {
                insert(curr[j]);
                answer++;
            }
        }

        // Copy current to previous
        for (int j = 0; j < currSize; j++)
            prev[j] = curr[j];

        prevSize = currSize;
    }

    return answer;
}
