bool hasAlternatingBits(int n) {
    while (n > 0) {
        int bit1 = n & 1;
        int bit2 = (n >> 1) & 1;

        if (bit1 == bit2) {
            return false;
        }

        n >>= 1;
    }

    return true;
}
