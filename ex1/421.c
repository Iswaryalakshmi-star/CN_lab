#include <stdio.h>
#include <stdlib.h>

// Instead of an absolute struct pointer, we track child nodes using integer indices.
// This allows us to keep everything inside a single, fast array pool.
typedef struct {
    int next[2];
} TrieNode;

// Pre-allocate a global static node pool to completely avoid the overhead of malloc() inside loops.
// Max nodes needed ~ numsSize * 31 bits = 200,000 * 31 = ~6,200,000.
#define MAX_NODES 6200005
TrieNode nodePool[MAX_NODES];
int nodeCount = 0;

// Resets our node pointer tracker for each new LeetCode test case
void resetTrie() {
    nodeCount = 0;
    nodePool[0].next[0] = -1;
    nodePool[0].next[1] = -1;
    nodeCount++;
}

// Inserts a new node into the pool
int createNode() {
    nodePool[nodeCount].next[0] = -1;
    nodePool[nodeCount].next[1] = -1;
    return nodeCount++;
}

void insert(int num) {
    int curr = 0; // Starts at root node index 0
    for (int i = 30; i >= 0; i--) {
        int bit = (num >> i) & 1;
        if (nodePool[curr].next[bit] == -1) {
            nodePool[curr].next[bit] = createNode();
        }
        curr = nodePool[curr].next[bit];
    }
}

int getMaxXOR(int num) {
    int curr = 0;
    int max_xor = 0;
    for (int i = 30; i >= 0; i--) {
        int bit = (num >> i) & 1;
        int target_bit = 1 - bit; // Greedily look for the opposite bit to maximize XOR
        
        if (nodePool[curr].next[target_bit] != -1) {
            max_xor |= (1 << i); 
            curr = nodePool[curr].next[target_bit];
        } else {
            curr = nodePool[curr].next[bit];
        }
    }
    return max_xor;
}

int findMaximumXOR(int* nums, int numsSize) {
    if (numsSize < 2) return 0;
    
    // Clear and prepare the static array structure
    resetTrie();
    
    // Step 1: Insert all numbers into the array-backed Trie
    for (int i = 0; i < numsSize; i++) {
        insert(nums[i]);
    }
    
    // Step 2: Traverse to find the highest XOR pairing configuration
    int maxXOR = 0;
    for (int i = 0; i < numsSize; i++) {
        int currentXOR = getMaxXOR(nums[i]);
        if (currentXOR > maxXOR) {
            maxXOR = currentXOR;
        }
    }
    
    return maxXOR;
}
