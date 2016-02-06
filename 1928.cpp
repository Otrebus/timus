/* 1928. Another Ecology Problem - http://acm.timus.ru/problem.aspx?num=1928
 *
 * Strategy:
 * Minmax + dynamic programming.
 *
 * Performance:
 * O(nk), runs the test suite in 0.015s using 344KB memory.
 */

#include <stdio.h>
#include <algorithm>

std::pair<int, int> F[10001], W[10001];

std::pair<int, int> minmax(int i, int n, int m, int k, std::pair<int, int>* W)
{
    std::pair<int, int> fmax = { -100000, 0 };
    for(int j = i-1; j >= i-k; j--)
    {
        if(j < 0)
        {
            if(m > fmax.first)
                fmax = { m, 0 };
            break;
        }
        std::pair<int, int> pz = { i-j + W[j].second, W[j].first };
        if(pz.first > fmax.first || 
           pz.first == fmax.first && pz.second < fmax.second)
            fmax = pz;
    }
    return fmax;
}

int main()
{
    int n, m, k;
    scanf("%d %d %d", &n, &m, &k);
    for(int i = 1; i <= n; i++)
    {
        F[i] = minmax(i, n, m, k, W);
        W[i] = minmax(i, n, m, k, F);
    }
    printf("%d %d", W[n]);
}