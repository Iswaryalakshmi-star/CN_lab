int totalHammingDistance(int* nums, int numsSize) {
    int ans = 0;

    for (int bit = 0; bit < 32; bit++) {
        int ones = 0;

        for (int i = 0; i < numsSize; i++) {
            if ((unsigned int)nums[i] & (1U << bit)) {
                ones++;
            }
        }

        int zeros = numsSize - ones;

        ans += ones * zeros;
    }

    return ans;
}
