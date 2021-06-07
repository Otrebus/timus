/* 1273. Tie - http://acm.timus.ru/problem.aspx?num=1273
 *
 * Strategy:
 * Sort the lines by the leftmost endpoints, and maintain a data structure ordered by the rightmost
 * endpoints (in this case a plain vector) where each index contains the solution of the problem for 
 * the lines that we have processed so far that have a rightmost endpoint below the position equal
 * to the index (actually the number of endpoints we use rather than throw away). Go through each
 * leftmost endpoint in the sorted list and update the data structure accordingly. This is of O(n^2)
 * complexity which is enough for a problem of such small size, but there is an O(n log n) algorithm
 * as below:
 * 
 * Sort lines by the left endpoints, and have a segment tree S or similar for point update/range 
 * maximum query for the right endpoints, where S.update(i, v) updates element i to equal value v,
 * and S.max(i) returns the maximum value of any element below i. For each line (i, j) in the sorted
 * list, do S.update(j, S.max(j)). The final result is then N-S.max(N).
 *
 * Performance:
 * O(N^2), using 392KB memory.
 */

#include <vector>
#include <iostream>
#include <algorithm>

int ans[100];

int main()
{
    int N;
    std::cin >> N;
    std::vector<std::pair<int, int>> v(N);
    for(int i = 0; i < N; i++)
        std::cin >> v[i].first >> v[i].second;
    // First we normalize the coordinates so we can index by them directly
    std::sort(v.begin(), v.end(), // Sort by rightmost endpoints
              [] (std::pair<int, int> a, std::pair<int, int> b) { return a.second < b.second; } );
    for(int i = 0; i < v.size(); i++) // Normalize rightmost endpoints
        v[i].second = i;
    std::sort(v.begin(), v.end()); // Sort by left endpoints
    for(int i = 0; i < v.size(); i++) // Normalize leftmost endpoints
        v[i].first = i;
    for(int i = 0; i < v.size(); i++) // The actual computation
        ans[v[i].second] = 1 + *std::max_element(ans, ans+v[i].second);
    std::cout << N - *std::max_element(ans, ans+N);
}