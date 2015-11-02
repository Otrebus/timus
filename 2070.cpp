/* 2070. Interesting Numbers - http://acm.timus.ru/problem.aspx?num=2070
 *
 * Strategy:
 * In essence, we are looking for all numbers within the given range that have a prime number of 
 * divisors. We use the number-of-divisors function d(n) = (k1+1)(k2+1)..(kn+1) for a number 
 * n = p1^k1*p2^k2*..*pn*kn where p1..pn are primes. The value of this function at some n is a prime
 * number only if all powers are zero except for one power which is one smaller than a prime number 
 * not equal to two. Hence, we identify all powers p^(k-1) where p and k are prime numbers (k > 2),
 * that lie within the requested range. This result is the "inverse" of the requested result so we
 * subtract this number from the size of the given range.
 *
 * Performance:
 * O(sqrt(R) log log R), because of the sieving step, running the test suite in 0.015s using 1992KB
 * memory.
 */

#include <iostream>

bool b[1000000];
unsigned long long p[1000000];
int np = 0;

void sieve(int N)
{
    for(unsigned long long i = 2; i < N; i++)
    {
        if(!b[i])
        {
            p[np++] = i;
            for(unsigned long long j = i*i; j > 0 && j < N; j += i)
                b[j] = true;
        }
    }
}

unsigned long long algo(unsigned long long L, unsigned long long R)
{
    unsigned long long tot = R-L+1;
    for(int i = 0; p[i]*p[i] <= R && i < np; i++)
    {
        int exp = 0;
        unsigned long long mul = 1;
        for(int j = 1; mul < R; j++)
        {
            while(exp++ < p[j] - 1)
            {
                mul *= p[i];
                if(mul > R)
                    break;
            }
            if(mul >= L && mul <= R)
                tot--;
            else if(mul > R)
                break;
            mul *= p[i];
        }
    }
    return tot;
}

int main()
{
    sieve(1000000);
    unsigned long long L, R;
    std::cin >> L >> R;
    std::cout << algo(L, R) << std::endl;
}