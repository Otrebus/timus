/* 1940. Not So Simple Years - http://acm.timus.ru/problem.aspx?num=1940
 *
 * Strategy:
 * Let A_k be the set of integers between a = A and b = A + B inclusive that are divisible by k.
 * For example, for a = 5 and b = 12, we have A_3 = { 6, 9, 12 }. What the problem asks us to do
 * (if we ignore the "ending with a number, one less than verifiable" part for a second, so we allow
 * prime numbers to be counted) is to calculate the sum B - |A_2 + A_3 + A_4 + ... + A_k|, where 
 * the latter term is the number of integers i that have some divisor in the numbers 2 through k,
 * and where the '+' operator is the union operation (this is done whenever we plus together sets).
 * The latter term is the same as the sum |A_2 + A_3 + A_5 + ... + A_p| where p is the greatest
 * prime <= k. Let S_p denote the union A_2 + A_3 + A_5 + ... + A_p. To calculate the size of this 
 * set we can recursively apply the principle of inclusion and exclusion:
 *
 *            |S_p| = |A_p + S_{p-1}| = |A_p| + |S_{p-1}| - |A_p intersect S_{p-1}|  (*)
 * 
 * Let's look at a specific example with a = 2, b = 32 and k = 5. To calculate |S_5| we first count
 * |A_5|, the number of integers divisible by 5 in the range, then recursively calculate |S_3|, and
 * then subtract the number of integers that are divisible both by 5 and some number in S_3. To see
 * how to calculate size of the latter set, consider the entire range:
 *
 *       2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32
 *
 * Then, sort out those integers divisible by 5 in the range to get A_5:
 *
 *      A_5:   5         10             15             20             25             30
 *
 * To find out which of these are divisible by some number in S_3, we divide every number in this
 * sequence by 5, adjust the endpoints to be 1 to 6, and use the procedure call to calculate S_3 
 * between those endpoints:
 *
 *  "A_5/5":   1         2              3              4              5              6
 *      S_3:             2              3              4                             6
 *
 * The four numbers 2, 3, 4, and 6 correspond to the original integers 10, 15, 20, and 30. These are 
 * the integers in A_5 that we already included in S_3 and will be removed by the third term in (*).
 * This method can then be used generally to calculate any given S_k.
 * 
 * Once we have the answer from the procedure above, we calculate B - S_p and then add the number of 
 * prime numbers between A and A + B, since we allowed them to be included in the S_p term in order to 
 * simplify the recursive call.
 *
 * Performance:
 * Too difficult to analyze, but the program runs the tests in 0.265s using 260KB memory.
 */

#include <iostream>
#include <vector>

bool b[302];
std::vector<int> p;

void sieve(int N) // Calculates the primes
{
    for(int i = 2; i <= N; i++)
    {
        if(!b[i])
        {
            p.push_back(i);
            for(int j = i*i; j > 0 && j <= N; j += i)
                b[j] = true;
        }
    }
}

int calc(int a, int b, int k) // Calculates |S_{p[k]}| between a and b
{
    if(b <= a || k == -1)
        return 0;
    return b/p[k]-a/p[k] + calc(a, b, k-1) - calc(a/p[k], b/p[k], k-1);
}

int main()
{
    int A, B, k;
    std::cin >> A >> B >> k;
    sieve(k+1);
    int ans = B - calc(A, A+B, p.size()-1);
    for(auto i : p)
        ans += i > A && i <= A+B;
    std::cout << ans;
}