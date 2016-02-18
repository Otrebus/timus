/* 1838. Samurai's Stroke - http://acm.timus.ru/problem.aspx?num=1838
 *
 * Strategy:
 * Cutting at the span outside the outermost props always drops those pieces of stem. Other than
 * those parts, consider each span between two props p1 and p2, p1 < p2. There are four intervals
 * Ij = [lj, rj] to consider:
 *
 *    1) p1 = l1 < r1 <= p2 - cutting here causes the right stem to flip ccw
 *    2) p1 = l2 < r2 <= p2 - cutting here causes the left stem to flip ccw
 *    3) p1 <= l3 < r3 = p2 - cutting here causes the right stem to flip cw
 *    4) p1 <= l4 < r4 = p2 - cutting here causes the left stem to flip cw
 *
 * We calculate these intervals and essentially take the union of all of them.
 *
 * Performance:
 * O(n), runs the test cases in 0.031s using 576KB memory (rated 4/339).
 */

#include <stdio.h>
#include <algorithm>

int A[100000];

int main()
{
    int l, n, ans;
    scanf("%d %d", &l, &n);
    for(int i = 0; i < n; i++)
        scanf("%d", &A[i]);
    ans = A[0] + l - A[n-1]; // Outside parts
    auto clamp = [] (int in, int b) { return std::min(b, std::max(in, 0)); };
    for(int i = 0; i + 1 < n; i++)
    {   // L = width of the union of I1 and I2, R = width of the union of I3 and I4
        int L = std::max(clamp(2*A[i+1]-l-A[i], A[i+1]-A[i]), clamp(A[0]*2-A[i], A[i+1]-A[i]));
        int R = std::max(clamp(A[i+1]-2*A[i], A[i+1]-A[i]), clamp(A[i+1]+l-2*A[n-1], A[i+1]-A[i]));
        ans += std::min(A[i+1]-A[i], L+R); // Add the width of the union of L and R to the answer
    }
    printf("%d", ans);
}