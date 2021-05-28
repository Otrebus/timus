/* 1883. Ent's Birthday - http://acm.timus.ru/problem.aspx?num=1883
 *
 * Strategy:
 * Sort the candles by x coordinate first, y coordinate second. Since the candles can't occupy the
 * very edge of the cake, we can form a cut from the top left corner of the cake, the bottom left,
 * then a cut along the bottom edge until we can make a vertical cut up until the kth candle, and
 * from there a cut that excludes all other candles from that column by cutting to the top edge of
 * the cake, but one unit to the left. (The last cut is only necessary if the kth candle does not
 * occupy the leftmost possible column.)
 *
 * Performance:
 * O(1), runs the tests in 0.015s using 148KB memory.
 */

#include <vector>
#include <stdio.h>
#include <algorithm>

struct pt { int x, y; } v[1000];
const int m = 1e9;

int main() {
    int n, k, x, y;
    scanf("%d %d", &n, &k);
    for(int i = 0; i < n; i++)
        scanf("%d %d", &v[i].x, &v[i].y);
    std::sort(v, v+n, [] (pt a, pt b) { return std::pair(a.x, a.y) < std::pair(b.x, b.y); });

    std::vector<pt> o = { {-m, m}, {-m, -m}, {v[k-1].x, -m}, v[k-1] };
    if(v[k-1].x > 1-m)
        o.push_back( { v[k-1].x-1, m } );

    printf("%d\n", o.size());
    for(auto q : o)
        printf("%d %d\n", q.x, q.y);
}
