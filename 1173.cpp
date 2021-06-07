/* 1173. Lazy Snail - http://acm.timus.ru/problem.aspx?num=1173
 *
 * Strategy:
 * Sort the homes by the atan2 angle around Wally's home. If any pair of points in the sorted list
 * forms a clockwise turn at any point, start iterating the list from the second point onwards.
 *
 * Performance:
 * O(Nlog N), runs the tests in 0.015s using 288KB memory.
 */

#include <iostream>
#include <cmath>
#include <algorithm>

using ll = long long;
using ld = long double;
ll x, y; // Wally's home

struct point {
    ll x, y;
    int i;
} points[1000];

ll conv(ld x) { // Converts the points to integers for perfect accuracy
    return ll(x*1000+0.5l*(x >= 0 ? 0.5 : -0.5));
}

bool lt(point& a, point& b) { // True if points a to b form a clockwise angle with the origin
    return a.x*b.y - a.y*b.x < 0;
}

int main() {
    int n, k = 0;
    ld a, b;

    std::cin >> a >> b >> n;
    x = conv(a), y = conv(b);

    for(int i = 0; i < n; i++) {
        ld x, y;
        std::cin >> x >> y >> points[i].i;
        points[i].x = conv(x-a), points[i].y = conv(y-b);
    }

    // Sort by the atan2 angle around the origin
    std::sort(points, points+n, [] (point& a,point& b) { return atan2(a.y,a.x) < atan2(b.y,b.x); });
    for(int i = 0; i < n-1; i++)
        if(lt(points[i], points[i+1])) // Find a possible point where we get a clockwise turn
            k = i+1;

    std::cout << 0 << std::endl;
    for(int i = 0; i < n; i++)
        std::cout << points[(i+k)%n].i << std::endl; // Iterate from the clockwise turn onwards
    std::cout << 0 << std::endl;
}
