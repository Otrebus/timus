/* 1444. Elephpotamus - https://acm.timus.ru/problem.aspx?num=1444
 *
 * Strategy:
 * Sort the points by atan2 around the first point and build the polygon by that order. If, at any
 * point, the polygon makes a >180 degree turn (which might make lines overlap), make the polygon
 * start (after the first point) right after the turn instead.
 *
 * Performance:
 * O(N log N), runs the tests in 0.187s using 636KB memory.
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

struct point {
    int x, y;
    int i;
} p[30000];
point pk;

int f(point& a, point& b) {
    if(a.x*b.y == a.y*b.x && a.x*b.x + a.y*b.y >= 0)
        // Points with the same angle are sorted outwards
        return a.x*a.x + a.y*a.y < b.x*b.x + b.y*b.y;
    return atan2(a.y, a.x) < atan2(b.y, b.x);
}

int main() {
    int n;
    std::ios::sync_with_stdio(false);
    std::cin >> n;

    for(int i = 0; i < n; i++) {
        int x, y;
        std::cin >> p[i].x >> p[i].y;
        p[i].i = i;
    }

    for(int i = n-1; i >= 0; i--)
        // Translate everything towards the first point
        p[i].x -= p[0].x, p[i].y -= p[0].y;

    std::sort(p, p+n, f);

    int s = 0;
    for(int i = 0; i < n-1; i++) {
        point p0 = p[0], p1 = p[i], p2 = p[i+1];
        int d1x = p1.x-p0.x, d2y = p2.y-p0.y, d1y = p1.y-p0.y, d2x = p2.x-p0.x;
        int x = d1x*d2y - d1y*d2x, d = d1x*d2x + d1y*d2y;
        if(x < 0 || x == 0 && d < 0) { // Opposite turn, start at that point instead
            s = i;
            break;
        }
    }

    std::cout << n << std::endl;
    std::cout << (p[0].i+1) << std::endl;
    for(int i = 0; i < n-1; i++)
        std::cout << (p[(s+i)%(n-1)+1].i+1) << std::endl;
}
