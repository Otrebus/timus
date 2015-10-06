/* 1860. Fiborial - http://acm.timus.ru/problem.aspx?num=1860
 *
 * Strategy:
 * The factors of F_i turn out to be equal to i^f_0 * (i-1)^f_1 * (i-2)^f_2 * ... * 2^f_(i-2), where
 * f_n is the normal additive Fibonacci term. This is easily seen by working out for example F_5,
 * and can be proven in the general case by induction. In order to extract the prime factors and
 * their powers, a sieve method is used, and finally the number of divisors can be calculated.
 *
 * Performance:
 * Something like O(n log^2n) - the sieve is O(n log n), but there is an inner loop that iterates
 * each number up to log n times. The solution runs the test cases in 0.078s using 8328KB memory.
 */

#include <stdio.h>

const int nprimes = 78498; // Number of primes up to 10^6
const int mod = 1000000007;
const int maxn = 1000001;
int fib[maxn];
bool p[maxn];      // Sieving table
int res[nprimes]; // The power of each prime in the result

int main()
{
    int ip = -1, n;
    scanf("%d", &n);
        
    fib[0] = fib[1] = 1;
    for(int i = 2; i <= n; i++) // Calculate (moduli of) fibonacci numbers
        fib[i] = (fib[i-1] + fib[i-2]) % mod;

    for(int i = 2; i <= n; i++) // Sieve of Eratosthenes
    {
        if(!p[i])
        {
            ip++;
            for(int j = i; j <= n; j += i)
            {
                p[j] = true;
                // Extract the largest power of the currently sieved prime factor that this number 
                // contains and multiply it with the fibonacci factor, for example 18^f_6 = 2^f_6 *
                // * 3^f_6 * 3^f_6, so the result vector gets 2*f_6 added to the entry for 3
                for(int m = j; m % i == 0; m /= i)
                    res[ip] = (res[ip] + fib[n-j]) % mod;
            }
        }
    }
    long long result = 1;
    // p^i*q^j*... has (i+1)*(j+1)*... divisors:
    for(int i = 0; i <= ip; i++)
        result = (result * (res[i] + 1)) % mod;

    printf("%lld\n", result);
    return 0;
}