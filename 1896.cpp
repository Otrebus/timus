/* 1896. War Games 2.1 - http://acm.timus.ru/problem.aspx?num=1896
 *
 * Strategy:
 * Use a Fenwick tree with a clunky "get Kth" function to circumvent the memory limit.
 *
 * Performance:
 * O(Nlog N), runs the tests in 0.343s using 4,116KB memory.
 */

#include <vector>
#include <algorithm>
#include <iostream>

std::vector<int> v;
int N, K, p;

void init() {
    // Inits the tree with a 1 at every index
    p = N-1;
    for(int i = 1; i <= 16; i *= 2)
        p |= p >> i;
    p++;
    v.resize(N + 1);

    for(int i = 1; i <= N; i++)
        v[i] = i & (-i);
}

int getKth(int k) {
    // Gets the position of the kth 1
    int rc = p, lc = p/2;
    do {
        if(k <= v[lc]) // Left half has k 1's
            rc = lc, lc -= (lc & -lc) >> 1;
        else { // Right child has k 1's
            k -= v[lc];
            // Try to split right child
            int d = lc + std::max(1, (rc-lc)/2);
            if(d <= N)
                lc = d;
            else { // Splitting point is outside the tree, so no info to guide us to split further
                lc++;

                while(lc + (lc & -lc) <= N) // We basically have to iterate here
                    lc += (lc & -lc);
            }
        }
    } while (rc != lc);
    return rc;
}

void dec(int i) {
    // Decrements (removes) the item at position i in the tree
    for(; i <= N; i += (i & -i))
        v[i]--;
}

int main() {
    std::cin >> N >> K;
    init();
    int i = 0, sum = 0, k = 1, n = N, r;
    do {
        k = 1 + (k-2 + K) % n;
        dec(r = getKth(k));
        sum ^= std::abs(r - ++i);
    } while(--n);

    std::cout << sum;
}
