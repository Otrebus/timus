/* 1689. Fisherman and Barbell - http://acm.timus.ru/problem.aspx?num=1689
 *
 * Strategy:
 * Scan the groove from the left to the right centimeter by centimeter, updating a counter of worms
 * currently crushed whenever the appropriate end of a barbell has reached a point where we recorded
 * the start of a worm or a worm length after the start of a worm.
 *
 * Performance:
 * O(L) where L is the maximum length of the groove. Runs the tests in 0.124s using 1,092KB memory.
 */

#include <iostream>

int A[400000];
int *a = A + 200000;

int main()
{
    std::ios::sync_with_stdio(false);
    int g, w, b, p, n, t, min = 1000000, ans, cur = 0;
    std::cin >> g >> w >> b >> p >> n;
    for(int i = 0; i < n; i++)
    {
        std::cin >> t;
        a[t] = 1;
    }
    for(int x = 0; x <= g; x++) // x is at the right end of the bar
    {
        if(cur < min && x >= b)
            ans = x, min = cur;
        // If a worm is longer than the internal distance of the bar, we don't want to count it 2x
        cur += a[x] + (b-2*p >= w ? a[x-b+p] - a[x+1-p-w] : 0) - a[x+1-b-w];
    }
    std::cout << (ans-b);
}