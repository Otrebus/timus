/* 2078. Bowling game - http://acm.timus.ru/problem.aspx?num=2078
 *
 * Strategy:
 * Straightforward; for the minimum, make the 10's spares, and for the maximum, make them strikes.
 *
 * Performance:
 * Constant, runs in 0.001s using 172KB memory.
 */

#include <algorithm>
#include <stdio.h>

int main()
{
    int A[10];
    for(int i = 0; i < 10; i++)
        scanf("%d", A+i);
    int min = 0, max = 0, i; 
    bool spare = false;
    for(i = 0; i < 9; i++)
    {
        min += A[i];
        spare = A[i] == 10;
    }
    min += A[i];
    if(A[i] > 20)
        min += 10*(spare ? 1 : 0);

    int strike = 0;
    for(i = 0; i < 9; i++)
    {
        max += A[i]*(1 + strike);
        strike = A[i] < 10 ? 0 : std::min(2, strike+1);
    }

    max += std::min(A[i], 10)*(1 + strike);
    if(A[i] > 10)
        max += std::min(10, (A[i]-10))*(1 + (strike == 2 ? 1 : strike == 1 ? 1 : 0)) 
            + std::max(A[i]-20, 0);
    printf("%d %d", min, max);
}