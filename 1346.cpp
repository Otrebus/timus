/* 1346. Intervals of Monotonicity - http://acm.timus.ru/problem.aspx?num=1346
 *
 * Strategy:
 * Greedy. Read the input numbers one by one and start a new segment when we have enough information
 * of which direction it is going, whenever we can't continue the old one.
 * 
 * Performance:
 * O(B-A), running the tests in 0.031s using 196KB memory, a comparably fast solution.
 */

#include <stdio.h>
#include <cmath>

int main()
{
    int a, b, state = 0, ans = 1, prev, x;
    scanf("%d %d %d", &a, &b, &prev);
    for(int i = 1; i < 1+b-a; i++)
    {
        scanf("%d", &x);
        if(!state && prev != x)
            state = std::abs(x-prev)/(x-prev);
        else if(x*state < prev*state)
            state = 0, ans++;
        prev = x;
    }
    printf("%d\n", ans);
    return 0;
}