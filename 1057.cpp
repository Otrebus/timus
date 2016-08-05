/* 1057. Amount of Degrees - http://acm.timus.ru/problem.aspx?num=1057
 *
 * Strategy:
 * Combinatorics. First, we can reduce the problem to the case where X = 1 since if the solution 
 * for some K and B is expressed F(X, Y), we have F(X, Y) = F(1, Y) - F(1, X-1). Thus, for any
 * number N, we want to count how many different numbers up to N contain K 1's in its base-B
 * representation. 
 *
 * Let's consider two different representative cases. First, N = 3102021 with base 4 and K = 3. 
 * We can dismiss all numbers 2000000 and above since they contain a digit that is neither 0 or 2.
 * For every number starting with a zero, we calculate how many ways we can add 3 1's to the
 * remaining 6 digits, (6 choose 3), and for every number starting with a one, we calculate the
 * number of ways we can add the remaining 2 1's to the remaining 6 digits (6 choose 2). We 
 * add these two results together for the answer.
 * 
 * To get the answer for a number starting with a 1, say 1102021, we calculate the number of 
 * numbers starting with a zero that contain 3 1's the same way, but for the numbers starting 
 * with a 1, we recurse to calculating the answer for the number 102021 with K = 2.
 *
 * Performance:
 * O(log^2 Y), runs the test cases in 0.015s using 168KB memory.
 */

#include <stdio.h>

int C(int n, int k) // n choose k
{
    long long c = 1;
    for(int i = 0; i < k; i++)
        c = c*(n-i)/(i+1);
    return c;
}

int F(int N, int K, int B)
{
    if(N < B) // Base case for a single digit; make sure K makes sense and return 1 or 0 accordingly
        return K <= 1 && N > 0 || N == 0 && K == 0;
    long long p = 1, k = 0;
    while(p*B <= N)
        p *= B, k++;
    if(N/p >= 2)
        return C(k, K-1) + (K ? C(k, K) : 0);
    else if(N/p == 1)
        return C(k, K) + (K ? F(N-p, K-1, B) : 0);
}

int main()
{
    int X, Y, K, B;
    scanf("%d %d %d %d", &X, &Y, &K, &B);
    printf("%d", F(Y, K, B) - F(X-1, K, B));
}