/* 1407. One-two, One-two - http://acm.timus.ru/problem.aspx?num=1407
 *
 * Strategy:
 * Every power 2^n divides some n-digit number of the form given in the problem statement (and the
 * following construction can serve as the inductive part of a proof), which we can express as 
 * either 2*k*2^n (call this the 'even form'), or (2*k + 1)*2^n (the 'odd form') for some k. If we
 * extend this number by one digit by adding 10^n = 2^n*5^n or 2*10^n = 2*2^n*5^n, we can get an
 * n+1-digit number divisible by 2^(n+1), by choosing the digit depending on the 'form' parity.
 * For the 'odd form' we extend the number with an 1:
 *   (2*k+1)*2^n + 2^n*5^n = 2k*2^n + (5^n + 1)*2^n = 2m*2^n = 2(m-1)*2^(n+1).
 * For the 'even form' we extend it with a 2:
 *    2k*2^n + 2*2^n*5^n = 2*(k+5^n)*2^n = (k+5^n)*2^(n+1). 
 * To figure out whether a number is of the odd or even form, we repeatedly use long division by 2.
 *
 * Performance:
 * O(n^3), running the tests in 0.001s using 160KB memory.
 */

#include <stdio.h>
#include <cstring>

char ans[100], r[2][100];

int main()
{
    int N, t = 0;
    scanf("%d", &N);
    for(int n = 0; n < N; n++)
    {
        std::memcpy(r[t], ans, N);
        for(int i = 0; i < n; i++, t = (t+1) % 2) // Divide n times ..
            for(int k = n, m = 0; k >= 0; k--)    // Long division by 2 
                r[(t+1)%2][k] = (r[t][k]+m*10)/2, m = (r[t][k]+m*10) % 2;
        ans[n] = 2 - r[t][0] % 2; // Next digit is 1 if the result is odd, 2 if it's even
    }
    for(int n = N-1; n >= 0; n--)
        printf("%d", ans[n]);
    return 0;
}