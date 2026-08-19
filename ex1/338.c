#include <stdio.h>
#include <stdlib.h>

/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
int* countBits(int n, int* returnSize) {
    // The size of the output array must be n + 1
    *returnSize = n + 1;
    
    // Allocate memory for the results array
    int* ans = (int*)malloc((*returnSize) * sizeof(int));
    if (ans == NULL) {
        return NULL;
    }
    
    // Base case: The binary representation of 0 has zero 1s
    ans[0] = 0;
    
    // Fill the array using the dynamic programming recurrence relation
    for (int i = 1; i <= n; i++) {
        // i >> 1 drops the last bit (effectively dividing i by 2)
        // i & 1 extracts the last bit (adds 1 if the number is odd, 0 if even)
        ans[i] = ans[i >> 1] + (i & 1);
    }
    
    return ans;
}
