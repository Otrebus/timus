/* 1259. How to Become Star? [sic] - http://acm.timus.ru/problem.aspx?num=1259
 *
 * Strategy:
 * We are looking for solutions of the form N*alpha = 2*k*pi, where 0 < alpha < pi, so the amount of
 * different stars that we can find is the number of solutions to alpha = 2*pi*k/N, for alphas < pi
 * (k < N/2), where for each k generating an alpha we want k/N in lowest terms (have a gcd of 1) 
 * since otherwise that would be a solution for another star of a lower amount of vertices, retraced
 * gcd(k, N) times.
 * 
 * Performance:
 * O(N log N), running the tests in 0.015s using 200KB memory.
 */

#include <stdio.h>
#include <algorithm>

int gcd(int a, int b)
{
    while(b != 0)
        std::swap(a, b), b = b % a;
    return a;
}

int main()
{
    int ans = 0, N;
    scanf("%d", &N);
    for(int i = 1; i <= N/2; i++)
        if(gcd(i, N) == 1)
            ans++;
    printf("%d\n", ans);
    return 0;
}