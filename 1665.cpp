/* 1665. Dean's Financial Pyramid - http://acm.timus.ru/problem.aspx?num=1665
 *
 * Strategy:
 * The problem asks us to construct the number N through some composition consisting of an uneven
 * amount of factors besides 1. We can always combine any of the odd factors in the prime 
 * factorization to an arbitrary odd factor, but no 2's can be combined into a factor since even 
 * factors above 2 are not allowed according to the problem description. Therefore, we divide out
 * all twos, and note if the remaining number is 1, prime, or a composition, which will allow us 
 * to add an additional 0, 1 or 2 (ore more) factors as required beyond the 2's.
 * 
 * Performance:
 * O(T sqrt(N)), running the tests in 0.015s using 204KB memory.
 */

#include <stdio.h>

bool solve(int N)
{
    int pow2 = 0;
    while(N % 2 == 0)
        N /= 2, pow2++;
    if(pow2 % 2 == 1)
    {
        if(N == 1)
            return true;
        else 
            for(int i = 2; i*i <= N; i++)
                if(N % i == 0)
                    return true;
        return false;
    }
    else if(pow2 % 2 == 0)
        return N > 1;
}

int main()
{
    int N, T;
    scanf("%d", &T);
    while(T--)
    {
        scanf("%d", &N);
        printf(solve(N) ? "YES\n" : "NO\n");
    }
}