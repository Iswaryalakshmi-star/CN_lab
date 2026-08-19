#include <stdio.h>

int findComplement(int num) {
    // 0 is a special case; its binary is "0" and complement is "1"
    if (num == 0) {
        return 1;
    }
    
    unsigned int mask = ~0;
    
    // Shift the mask left until it aligns with the highest set bit of num
    while (num & mask) {
        mask <<= 1;
    }
    
    // ~mask creates 1s for the length of num. 
    // XOR (^) flips the bits of num within that length.
    return num ^ ~mask;
}

