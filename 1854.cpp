/* 1854. Negotiations with Parthians - http://acm.timus.ru/problem.aspx?num=1854
 *
 * Strategy:
 * Find all prime factors p1, p2, ... up to the cube root of the input number. Above this there are
 * at most two divisors remaining (since they are larger than the cube root). Next, (implicitly) 
 * calculate the number of divisors p1^(m1+1)+p2^(m2+1)+.. where mx is the multiplicity of each 
 * prime factor. To get an odd number of divisors, divide from the input the base of each
 * even factor of the number of divisors function. Then consider the two possible divisors above
 * the cube root - if they are unequal they have multiplicity 1 and thus contribute even factors,
 * into the number-of-divisor function so they are removed similarly.
 *
 * Performance:
 * O(n^(1/3)), runs in 0.031s and uses 192 KB of memory; one of the fastest solutions.
 */

#include <stdio.h>

long long sqrt(long long num) // https://en.wikipedia.org/wiki/Integer_square_root
{
    long long n = num;
    long long n1 = 1 + num / 2;
    while (n1 < n)
    {
        n = n1;
        n1 = (n + (num / n)) / 2;
    }
    return n;
}

int main()
{
    long long N;
    scanf("%lld", &N);
    long long ans = N;
    for(long long i = 2; i*i*i <= N; i++)
    {
        if(N % i == 0)
        {
            int k = 0;
            for(; N % i == 0; N /= i)
                k++; // Count multiplicity of this prime factor
            if(k % 2 == 1) // If it contributes to an even factor in the #divisors function,
                ans /= i;  // shave away from the answer
        }
    }
    long long r = sqrt(N); 
    if(r*r != N) // If what remains doesn't have multiplicity 2, just disregard it
        ans /= N;
    printf("%lld", ans);
    return 0;
}