/* 1326. Bottle Taps [sic] - http://acm.timus.ru/problem.aspx?num=1326
 *
 * Strategy:
 * Let a combination of caps be denoted by a binary number m, and let A[m] be the cheapest possible
 * price to get hold of those caps. Then, for each offer r, we can update A[m] by taking the cost of
 * the combination without the offer (called 'remainder' below) and adding the cost of the offer,
 * like so:
 *
 * Combination:   mmm m m mm mm, cost = 30
 * Offer:         ff fff   f   , cost = 15
 * Remainder:       m   m m  mm, cost = 13 (calculated earlier).
 * 
 * In this example we see that we have a cheaper way of acquiring the original combination by
 * choosing the offer f together with the (offers that make up the) remainder. This is calculated
 * recursively, using memoization.
 *
 * Performance:
 * O(m2^n), finishing the test suite in 1.076s using 4484KB memory. This can probably be improved
 * by doing the calculation iteratively bottom up instead of recursively top down.
 */

#include <iostream>
#include <vector>
#include <algorithm>

const int INF = 0x7FFFFFFF;
int A[1 << 20];
std::vector<std::pair<int, int>> v; // Offers represented by { bitmask, cost }

int solve(int m)
{
    if(!m || A[m] != INF) // If calculated earlier, or empty
        return A[m];
    for(int i = 0; i < v.size(); i++)
        if((~v[i].first & m) != m)
            A[m] = std::min(A[m], solve(~v[i].first & m) + v[i].second);
    return A[m];
}

int main()
{
    std::ios::sync_with_stdio(false);
    int n, m, k, x, y, c;
    std::cin >> n;
    for(int i = 1; i < (1 << n); i++)
        A[i] = INF;
    for(int i = 0; i < n; i++) // We count the full bottles as any other offer
        std::cin >> x, v.push_back( { 1 << i, x } );
    std::cin >> m;
    for(int i = 0; i < m; i++)
    {
        int M = 0;
        std::cin >> c >> k;
        while(k--)
            std::cin >> y, M |= (1 << (y-1)); // Parse offers into bitmasks
        v.push_back( { M, c } );
    }
    m = 0;
    std::cin >> k;
    while(k--)
        std::cin >> x, m |= (1 << (x-1)); // Parse wanted caps into bitmasks
    solve(m);
    std::cout << A[m] << std::endl;
    return 0;
}