/* 1431. Diplomas. - http://acm.timus.ru/problem.aspx?num=1431
 *
 * Strategy:
 * Greedy. For each 1 <= c <= 30, fill in how many different kinds of diplomas that there are
 * exactly c of. Go through this array of c's, and dedicate one wall for every matchable adjacent
 * element greater than zero, and one wall for all the ones left out.
 *
 * Performance:
 * O(nk) where n is the size of the input array and k is the largest number of a single kind of
 * diploma. Runs the tests in 0.001s using 196KB memory.
 */

#include <stdio.h>
#include <algorithm>

int c[31];

int main()
{
    int N, ans = 0;
    scanf("%d", &N);
    while(N--)
    {
        scanf("%d", c);
        c[*c]++;
    }
    for(int i = 2; i <= 30; i++)
    {
        int min = std::min(c[i], c[i-1]);
        c[i] -= min;
        c[i-1] -= min;
        ans += min;
    }
    for(int i = 1; i <= 30; i++)
        ans += c[i];
    printf("%d\n", ans);
}