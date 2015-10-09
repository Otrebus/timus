/* 1356. Something Easier - http://acm.timus.ru/problem.aspx?num=1356
 *
 * Strategy:
 * Use the Goldbach conjecture - every even number can be expressed as the sum of two prime numbers.
 * Uneven numbers can only be the sum of two prime numbers if one is 2; other than that we can
 * subtract 3 and use the conjecture on the remaining even term. There are a few special cases
 * taken care of. To actually find the primes in question, a brute-force search is performed. 
 *
 * Performance:
 * Roughly O(T log^2 N), since the distance between primes is logarithmic and we need to find two of
 * them (one high, one low) for each input number. Smaller primes have a smaller distance so this
 * bound is probably not tight. Moreover, this can be made faster by including a table of
 * precalculated primes. Runs in 0.218s using 200KB memory.
 */

#include <stdio.h>
#include <vector>

inline bool isPrime(int n)
{
    if(n == 1)
        return false;
   for(int i = 2; i*i <= n; i++)
       if(n % i == 0)
           return false;
   return true;
}

int main()
{
    int N, T;
    scanf("%d", &T);
    while(T--)
    {
        scanf("%d", &N);
        if(N < 4)
            printf("%d\n", N);
        else if(isPrime(N))
            printf("%d\n", N);
        else if(N % 2 == 0)
        {
            if(isPrime(N-2))
                printf("2 %d\n", N-2);
            else
            {
                for(int k = 3; ; k += 2)
                {
                    if(isPrime(k) && isPrime(N-k))
                    {
                        printf("%d %d\n", k, N-k);
                        break;
                    }
                }
            }
        }
        else
        {
            if(isPrime(N-2))
                printf("2 %d\n", N-2);
            else
            {
                N-=3;
                for(int k = N-3; ; k -= 2)
                {
                    if(isPrime(k) && isPrime(N-k))
                    {
                        printf("3 %d %d\n", k, N-k);
                        break;
                    }
                }
            }
        }
    }
    return 0;
}