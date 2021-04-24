/* 1817. Merry-go-round - http://acm.timus.ru/problem.aspx?num=1817
 *
 * Strategy:
 * Dynamic programming. For a carousel distribution of n children, represent it as a bitmask where
 * each child is a set bit and map it to the statistical frequency of such a bitmask. Then produce
 * the equivalent map for n+1 children by "inserting" a child (1) to each carousel bitmask and
 * moving it up the least significant digits until it reaches a 0 (empty seat) and adding the
 * frequency of the old bitmask to the new bitmask. At the end we can use the formula for expected
 * values to calculate the answer.
 * 
 * Performance:
 * O(N*2^N), runs the tests in 0.125s using 20,736KB memory.
 */

#include <stdio.h>
#include <memory>
#include <vector>
#include <bitset>

using ull = unsigned long long;
using uint = unsigned int;

long double A[2][1 << 20], *a = A[0], *b = A[1]; // The frequency map
std::vector<int> p[21]; // Maps n to the numbers with n bits set

uint rot(uint m, int n, int j) { // Rotate bitmask m which is n bits big, j steps
    return m >> j | ((m & ((1 << (j)) - 1)) << (n-j));
}

int lsb(int x) { // Returns the position of the smallest set bit
    return (x) & (-x);
}

int main() {
    int N;
    scanf("%d", &N);

    A[0][0] = 1;

    for(int i = 0; i < (1 << N); i++)
        p[std::bitset<32>(i).count()].push_back(i);

    for(int n = 0; n < N; n++) {
        long double t = 0; // Total frequency
        // For every carousel configuration of n children
        for(auto m : p[n]) {
            int m1 = lsb(~m) | m; // Put the child on the first free horse
            for(int j = 0; j < N; j++)
                // Rotate the carousel (bitmask) around and to the new frequency
                b[rot(m1, N, j)] += a[m];
            t += a[m];
        }

        long double ans = 0;
        for(auto m : p[n])
            // a[m]/t is the probability (freq/total freq) and log2(lsb) is the time you wait
            ans += log2(lsb(~m))*a[m]/t;
        printf("%Lf\n", ans);

        std::memset(a, 0, sizeof(A[0]));
        std::swap(a, b);
    }
}
