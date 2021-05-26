/* 2123. Knapsack - http://acm.timus.ru/problem.aspx?num=2123
 *
 * Strategy:
 * Brute-force dfs with memoization, using the fact that 2^n is greater than the sum of all smaller
 * powers of 2 to prune the search.
 *
 * Performance:
 * Hard to derive the complexity, runs the tests in 0.015s using 2,504KB memory.
 */

#include <stdio.h>
#include <vector>
#include <unordered_map>

using ull = unsigned long long;
ull v[60];

struct ans
{
    ull W; int i;
 
    std::size_t operator() (const ans &node) const {
        return std::hash<ull>()(node.W) ^ std::hash<int>()(node.i);
    }
    bool operator==(const ans &p) const { return W == p.W && i == p.i; }
};

std::unordered_map<ans, ull, ans> m; // Memo array

ull dfs(ull W, int i) { // Number of ways to make mass W with weights i and below
    if(m.find({ W, i }) != m.end()) // Already calculated
        return m[{W, i}];
    if(i < 0) // The end of the dfs, return 1 if we exhausted the weights and formed the mass
        return W == 0;
    if(v[i] < W/4) // No way for two each of all remaining weights to form the mass
        return 0;

    ull sum = 0;
    for(ull k = 0; k <= 2; k++) // Number of weights of type i to use
        if(W >= k*v[i])
            sum += dfs(W - k*v[i], i - 1);
    return m[{ W, i }] = sum;
}

int main() {
    ull W, n, x;
    scanf("%llu %llu", &n, &W);
    for(int i = 0; i < n; i++)
        scanf("%llu", &v[i]);
    printf("%llu", dfs(W, n-1));
}
