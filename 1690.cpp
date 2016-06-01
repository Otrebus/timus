/* 1690. Army of Mages - http://acm.timus.ru/problem.aspx?num=1690
 *
 * Strategy:
 * We want to pick those n points where if we pick any pair of points p1 and p2, |p1.x - p2.x| and
 * |p1.y - p2.y| are both even (this way the line between any such two such points will hit an
 * integer coordinate in the middle), so we conceptually map the coordinate grid into four buckets
 * like so:
 * 
 * 1 2 1 2 1 2
 * 3 4 3 4 3 4
 * 1 2 1 2 1 2
 * 3 4 3 4 3 4
 * 1 2 1 2 1 2
 * 3 4 3 4 3 4
 * 
 * We output the contents of whichever bucket ends up with the most points (up to n points).
 *
 * Performance:
 * O(n), runs the tests in 0.046s using 584KB memory.
 */

#include <algorithm>
#include <iostream>
#include <vector>

int main()
{
    using vi = std::vector<int>;
    vi v[4];
    int n, x, y;
    std::cin >> n;
    for(int i = 1; i <= 5*n; i++)
        std::cin >> x >> y, v[(x & 1) | ((y & 1) << 1)].push_back(i);
    auto max = std::max_element(v, v+4, [] (vi& a, vi& b) { return a.size() < b.size(); });
    std::cout << "OK\n";
    for(int i = 0; i < n; i++)
        std::cout << (*max)[i] << " ";
}