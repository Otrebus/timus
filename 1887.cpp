/* 1887. Frequent Flyer Card - http://acm.timus.ru/problem.aspx?num=1887
 *
 * Strategy:
 * Percolate the given probabilities through the modulos and solve the Coupon Collector's problem
 * at the end.
 *
 * Performance:
 * O(n), runs the tests in 0.015s using 272KB memory.
 */

#include <iostream>
#include <cmath>
#include <iomanip>

using ld = long double;

int A[3][26];
int *a = &A[0][0], *b = &A[1][0], *c = &A[2][0];

ld E[1024];

int cnt(int m) {
    int n = 0;
    for(int i = 0; i < 10; i++)
        n += bool(m & (1 << i));
    return n;
}

int main() {
    int n;
    std::cin >> n;
    for(int i = 0; i < 3; i++)
        for(int j = 0; j < n; j++)
            std::cin >> A[i][j];

    ld p[11], np[11], r[11], q[11];

    for(int i = 0; i < n; i++) {
        std::memset(r, 0, sizeof(r));
        std::memset(np, 0, sizeof(np));

        // Record where each digit ends up
        for(int x = 0; x < 10; x++)
            r[(x*x*a[i] + x*b[i] + c[i]) % 11] += 0.1l;

        if(!i)
            for(int i = 0; i < 11; i++)
                p[i] = r[i];
        else { // Cross-multiply the probabilities
            for(int u = 0; u < 11; u++) {
                for(int v = 0; v < 11; v++) {
                    int res = (u*v) % 11;
                    np[res] += p[u]*r[v];
                }
            }
            std::memcpy(p, np, sizeof(p));
        }
    }
    std::memset(q, 0, sizeof(q));

    for(int i = 0; i <= 10; i++)
        q[i%10] += p[i];

    // One formula for solving the coupon collector problem
    ld E = 0;
    for(int m = 1; m < 1024; m++) {
        ld pl = 0;
        for(int i = 0; i < 10; i++)
            if(m & (1 << i))
                pl += q[i];
        E += (cnt(m) % 2 ? 1 : -1)/pl;
    }

    std::cout << std::setprecision(20) << (std::isnan(E) ? -1 : E);
}
