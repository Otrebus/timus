/* 1132. Square Root - http://acm.timus.ru/problem.aspx?num=1132
 *
 * Strategy:
 * Naive iteration. Sort the input by n, and for each n iterate over all x < n to map every square
 * to its root, before applying the map to the input data.
 *
 * Performance:
 * O(N*p(N) + K) where p(n) is the prime-counting function and N is the upper limit of n, runs the
 * tests in 0.514s using 3,280KB memory.
 */

#include <vector>
#include <stdio.h>
#include <cstring>

const int maxn = 32767;

std::vector<std::pair<int, int>> k[maxn+1]; // Maps each prime to the input positions querying it

int main()
{
    int K, a, n;
    scanf("%d", &K);
    for(int i = 0; i < K; i++)
    {
        scanf("%d %d", &a, &n);
        k[n].push_back({i, a % n});
    }
    std::vector<std::pair<int, int>> output(K+1);
    for(int n = 0; n <= maxn; n++)
    {
        int r[maxn+1]; // The map that we fill in iteratively
        std::memset(r, 0, sizeof(r));
        if(!k[n].empty())
        {
            for(int x = 0; x < n; x++)
                r[(x*x)%n] = x;
            for(auto p : k[n])
                output[p.first] = std::make_pair((n-r[p.second])%n, r[p.second]);
        }
    }
    for(int i = 0; i < K; i++)
        if(!output[i].first)
            printf("No root\n");
        else if(output[i].first == output[i].second)
            printf("%d\n", output[i].first);
        else
            printf("%d %d\n", output[i].first, output[i].second);
}