/* 1292. Mars Space Stations - http://acm.timus.ru/problem.aspx?num=1292
 *
 * Strategy:
 * Naive.
 *
 * Performance:
 * O(TN) upper bound, runs the test cases in 0.031s using 196KB memory.
 */

#include <stdio.h>

int F(int n)
{
    int ret = 0;
    for(; n; n/= 10)
        ret += (n % 10)*(n % 10)*(n % 10);
    return ret;
}

int main()
{
    int T, N, K, L;
    scanf("%d", &T);
    int prev = 0;
    while(T--)
    {
        scanf("%d %d %d", &N, &K, &L);
        for(int i = 1; i < N; i++)
        {
            prev = K;
            K = F(K);
            if(K == prev)
                break;
        }
        printf("%d\n", K-L);
    }
    return 0;
}