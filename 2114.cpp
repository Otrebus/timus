/* 2114. My craft - http://acm.timus.ru/problem.aspx?num=2114
 *
 * Strategy:
 * We're being asked to calculate the maximum sum of digits that can be added inside the shape of
 * an axis-aligned triangle at any point in the grid, so we calculate that sum at point (1, 1)
 * and then use horizontal, vertical and diagonal prefix sums to quickly update the sum as we
 * move the triangle around vertically and horizontally. For example, if we move the triangle
 * right we add and subtract the appropriate +es and -es in constant time:
 *
 *   . . . . . .   . . . . . .
 *   . o . . . .   . - + . . .
 *   . o o . . .   . - o + . .
 *   . o o o . .   . - o o + .
 *   . o o o o .   . - o o o +
 *   . . . . . .   . . . . . .
 *
 * Performance:
 * O(qmn); runs the test cases in 0.187s using 10,280KB of memory.
 */

#include <stdio.h>
#include <algorithm>

using ll = long long;

ll F[3][1003][501]; // Prefix sums
ll A[501][501]; // Grid
 
long long sum(int f, int t, int i, int j) { // Calculates a range sum
    return F[f][t][j] - F[f][t][i-1];
}

void acc(int f, int t, int i, int j, ll sum = 0) { // Calculates a prefix sum
    for(int k = i; k <= j; k++)
        sum = (F[f][t][k] += sum);
}

int main() {
    int n, m, q, k;
    scanf("%d %d", &n, &m);

    for(int x = 1; x <= n; x++)
        for(int y = 1; y <= m; y++)
            scanf("%lld", &A[x][y]);

    // Assign the initial values of the prefix sums
    for(int x = 1; x <= n; x++) {
        for(int y = 1; y <= m; y++) {
            F[0][y][x] = A[x][y];
            F[1][x][y] = A[x][y];
            F[2][x+y][x] = A[x][y];
        }
    }

    // Accumulate the prefix sums
    for(int x = 1; x <= n; x++)
        acc(1, x, 1, m);
    for(int y = 1; y <= m; y++)
        acc(0, y, 1, n);
    for(int d = 1; d <= m+n+2; d++)
        acc(2, d, 1, n);

    scanf("%d", &q);
    while(q--) {
        ll ans = 0;
        scanf("%d", &k);
        ll Sh = 0;

        // Calculate the initial triangle
        for(int x = 1; x <= n; x++)
            for(int y = 1; y <= m; y++)
                if(x + y <= 2 + k)
                    Sh += A[x][y];

        int x0m = 1, y0m = 1;
        for(int x0 = 1; x0 <= n; x0++) {
            ll Sv = Sh;

            // Step vertically
            for(int y0 = 1; y0 <= m; y0++) {
                if(Sv > ans)
                    x0m = x0, y0m = y0, ans = Sv;

                // Add the diagonal and subtract the horizontal
                Sv += sum(2, std::min(m+n+2, x0+y0+k+1), x0, std::min(x0+k, n));
                Sv -= sum(0, y0, x0, std::min(x0+k, n));
            }

            // Step horizontally; add the diagonal and subtract the vertical
            Sh += sum(2, std::min(m+n+2, x0+k+2), x0+1, std::min(x0+k+1, n));
            Sh -= sum(1, x0, 1, std::min(1+k, m));
        }
        printf("%lld %d %d\n", ans, x0m, y0m);
    }
}
