/* 1554. Multiplicative Functions - http://acm.timus.ru/problem.aspx?num=1554
 *
 * Strategy:
 * If we calculate the values of G(i) from i = 1 up to i = N in turn, for each such calculation we
 * use the earlier calculated values to find the value for G(i). For example, for i = 6:
 *
 *   F(1)G(6) + F(2)G(3) + F(3)G(2) + F(6)G(1) = 0
 * 
 * The only unknown here is G(6), so this is just the equation G(6) + B = 0 (since F(1) = 1). 
 *
 * Performance:
 * O(Nlog N), runs the tests in 0.031s using 1,148KB memory.
 */

#include <stdio.h>
#include <vector>
#include <numeric>

const int mod = 2007, maxn = 10000;

std::vector<int> divs[maxn+1];

int F[maxn+1];
int G[maxn+1] = { 0, 1 };

int main()
{
    int N;
    scanf("%d", &N);
    for(int i = 1; i <= N; i++)
        scanf("%d", F+i);
    for(int i = 1; i <= N; i++) // Sieve of Eratosthenes
        for(int j = i; j <= N; j += i)
            divs[j].push_back(i);
    for(int i = 2; i <= N; i++) // This calculates G(i) = -B as in the header comment
        G[i] = (mod - std::accumulate(divs[i].begin(), divs[i].end(), 0, 
                                      [i](int a, int b){ return (a + F[i/b]*G[b]%mod)%mod; }))%mod;
    for(int i = 1; i <= N; i++)
        printf("%d ", G[i]);
}