/* 1343. Fairy Tale - http://acm.timus.ru/problem.aspx?num=1343
 *
 * Strategy:
 * Straightforward. Extend the given number with zeroes, test for primality, and increment.
 *
 * Performance:
 * Runs the tests in 0.031s using 152,KB memory.
 */

#include <stdio.h>
#include <stdlib.h>

bool isPrime(long long num) {
    if(num == 1 || num % 2 == 0)
        return false;
    for(long long i = 3; i*i <= num; i += 2)
        if(num % i == 0)
            return false;
    return true;
}

int main() {
    char c[14];
    int N;
    scanf("%d", &N);
    if(N)
        scanf("%s", c);

    long long num = atoll(c);
    for(int i = 0; i < 12-N; i++) // Extend with zeroes
        num *= 10;
    
    while(!isPrime(num)) // Increment until prime
        num++;

    printf("%012lld", num);
}
