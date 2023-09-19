/* 2163. Interview - http://acm.timus.ru/problem.aspx?num=2163
 *
 * Strategy:
 * The maximum number achievable is the minimum of the row and column sums of the row and column
 * of the element, and the minimum number is bound by the ability of the remainder of the column
 * sums be able to fulfill the row sum, and vice versa. See below.
 *
 * Performance:
 * O(n+m), runs the tests in 0.015s using 144KB memory.
 */

#include <stdio.h>
#include <numeric>

using ll = long long;
const int maxn = 1000;
ll A[maxn][maxn];
ll c[maxn], r[maxn];

int main() {
    int n, m, a, b;
    scanf("%d %d %d %d", &n, &m, &a, &b);
    a--, b--;
    for(int i = 0; i < n; i++)
        scanf("%lld", &r[i]);

    for(int j = 0; j < m; j++)
        scanf("%lld", &c[j]);

    ll R = std::accumulate(r, r+n, 0ll);
    ll C = std::accumulate(c, c+m, 0ll);

    if(R != C)
        return printf("0"), 0;

    ll max = std::min(r[a], c[b]); // This one is straightforward, but also
    // If we remove a number from a row, the rest of the column sums need to at least
    // be able to produce the remaining sum of that row
    ll min = std::max(std::max(0ll, r[a]-(C-c[b])), std::max(0ll, c[b]-(R-r[a])));

    printf("%lld", 1+max-min);
}
