/* 1212. Battleship - http://acm.timus.ru/problem.aspx?num=1212
 *
 * Strategy:
 * For each ship, add all forbidden points around the ship and inside the ship to two different
 * arrays which we sort in horizontal major order and vertical major order and iterate through.
 * The distance between any two points can then be used to determine how many ships fit in between
 * those points. For example, between these two points and considering ships of size 2:
 *
 *   .+...
 *   .....
 *   ...+.
 *
 * We can fit 8 ships between them horizontally, iterating row by row, and 4 ships vertically,
 * iterating column by column. If we do this procedure for all forbidden points we get the overall
 * answer.
 *
 * Performance:
 * O(L), runs whatever tests are used in 0.015s using 432KB memory.
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>

int N, M, L, K;

struct point { int x, y; };

int add(point a, point b, bool reverse)
{
    int W = reverse ? N : M; // The width of this row (actually column if reverse == true)
    if(reverse) // We're iterating vertically
        std::swap(a.x, a.y), std::swap(b.x, b.y);
    if(a.y == b.y) // Same row, easy
        return std::max(0, b.x-a.x-K);
    else // Different rows: handle all rows in between, right of the upper and left of the bottom
        return std::max(0, (b.y-a.y-1))*std::max(0, 1+W-K) 
             + std::max(0, b.x-K) + std::max(1+W-a.x-K, 0);
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::vector<point> v, h;
    std::cin >> N >> M >> L;
    while(L--)
    {
        int x, y, l;
        char c;
        std::cin >> x >> y >> l >> c;
        point lr, ul = { std::max(1, x-1), std::max(1, y-1) }; // Upper left, lower right extremes
        if(c == 'V')                                           // of the box of forbidden points
            lr = { std::min(x+1, M), std::min(y+l, N) };       // around the ship
        else
            lr = { std::min(x+l, M), std::min(y+1, N) };
        for(int x = ul.x; x <= lr.x; x++) // Add all points inside the box
            for(int y = ul.y; y <= lr.y; y++)
                v.push_back( { x, y } ), h.push_back( { x, y });

    }
    h.insert(h.end(), { { 0, 1 }, { M+1, N } }); // Sentinel points
    v.insert(v.end(), { { 1, 0 }, { M, N+1 } });
    std::cin >> K;
    std::sort(v.begin(), v.end(), [] (point& a, point& b)
                                      { return a.x == b.x ? a.y < b.y : a.x < b.x; });
    std::sort(h.begin(), h.end(), [] (point& a, point& b)
                                     { return a.y == b.y ? a.x < b.x : a.y < b.y; } );
    int ans = 0;
    for(int i = 1; i < h.size(); i++)
        ans += add(h[i-1], h[i], false);
    for(int i = 1; i < v.size(); i++)
        ans += add(v[i-1], v[i], true);
    std::cout << (K == 1 ? ans/2 : ans) << std::endl; // Single-point ships were counted twice
}