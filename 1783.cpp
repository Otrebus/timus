/* 1783. Nuclear Arms Race. - http://acm.timus.ru/problem.aspx?num=1783
 *
 * Strategy:
 * Naive.
 *
 * Performance:
 * O(mn), runs in 0.015s using 280KB memory.
 */

#include <stdio.h>
#include <algorithm>

int main()
{
    int n, m;
    scanf("%d %d", &n, &m);
    int* v = new int[n+m];
    for(int i = 0; i < n+m-1; i++)
        scanf("%d", v+i);
    int h = 0;
    for(int i = 0; i < n; i++)
    {
        int k = -100000;
        for(int j = i; j < i+m; j++)
            k = std::max(k, (v[j]-h)/(1+j-i) + (v[j]-h > 0 ? (v[j]-h)%(1+j-i) != 0 ? 1 : 0 : 0));
        printf("%d ", k);
        h += k;
    }
    return 0;
}