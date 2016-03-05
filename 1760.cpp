/* 1760. Infernal Work - http://acm.timus.ru/problem.aspx?num=1760
 *
 * Strategy:
 * Straightforward; from 1 to n, jump forward a steps and count how many junctions of b we pass
 * in each such span of a. Then calculate how many ways we can form pairs of ties from different
 * spans of b within this span of a.
 *
 * Performance:
 * O(n/a), runs the test cases in 1.419s using 188KB memory.
 */

#include <stdio.h>
#include <algorithm>

int main()
{
    long long a, b, n;
    long long ans = 0, l;
    scanf("%lld %lld %lld", &a, &b, &n);
    for(int p = 0; p < n; p += l)
    {
        l = std::min(a, n-p);
        int f = std::min(l, b - p%b), r = l - f, m = r/b;
        ans += (b*m*b*(m-1))/2 + (f*(r-m*b)) + m*b*(f+(r-m*b));
    }
    printf("%lld", ans);
}