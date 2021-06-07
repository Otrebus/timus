/* 2047. Maths - http://acm.timus.ru/problem.aspx?num=2047
 *
 * Strategy:
 * First, write a program that by brütë fôrcê \m/ finds some sum(k), for k = 100000, that satisfies
 * the recurrence given by the problem,
 *    sum_(k-1) = sum(k) - div(sum(k)), and a_k = div(sum(k))
 * (where sum(j) is a_1 + .. + a_j, and div(j) is the number of divisors of j) and then use this 
 * sum(k) and recurrence to construct the answer sequence.
 *
 * Performance:
 * O(m log m), where m = sum(k), since the first double loop iterates m + (m-1)/2 + (m-2)/3 + ... =
 * m + m/2 + m/3 + ... - 1/2 - 2/3 - 3/4 - ... = O(m log m) - O(m) = O(m) times. The program runs 
 * the test cases in 0.14s and uses 6796KB memory.
 */

#include <stdio.h>

int ndivs[1586336], ans[100001];

int main()
{
    int n, sum = 1586335;
    for(int i = 1; i <= sum; i++) // Calculates the number of divisors
        for(int j = i; j <= sum; j += i)
            ndivs[j]++;

    for(int i = 100000; i > 0; i--)
        ans[i] = ndivs[sum], sum -= ans[i];

    scanf("%d", &n);
    for(int i = 1; i <= n; i++)
        printf("%d ", ans[i]);
}