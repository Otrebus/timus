/* 1927. Herbs and Magic - http://acm.timus.ru/problem.aspx?num=1927
 *
 * Strategy:
 * If the segments are not parallel: From each segment endpoint, form a line perpendicular to that
 * segment, calculate the four intersections of these four lines, sort the intersections by their
 * atan2 angle around the centroid and use the shoelace formula to calculate the area of the quad.
 * 
 * If the segments are parallel: Sort them by their intersection with the X-axis (or Y-axis if they
 * are horizontal) and return an infinite area iff the first two lines come from the different
 * segments and if the second and third lines are not collinear.
 *
 * Performance:
 * O(1), runs the tests in 0.015s using 272KB memory.
 */

#include <iostream>
#include <algorithm>
#include <iomanip>

using ll = long long;
using ld = long double;

template<typename T> struct vec {
    T x, y;
    vec operator-(const vec& v) const { return { x-v.x, y-v.y }; }
    vec operator+(const vec& v) const { return { x+v.x, y+v.y }; }
    vec operator*(ld t) const { return { x*t, y*t }; }
};

template<typename T> struct line {
    // A line starts at point p (here, segment endpoint) and extends in the direction of v
    vec<T> p, v;
    int i; // The segment the line belongs to
};

bool f(line<ll> a, line<ll> b) {
    // Returns true if line a intersects the x-axis before line b
    if(a.v.y == 0)
        return a.p.y < b.p.y;
    ld x = b.v.y*a.v.y*a.p.x - b.v.y*a.v.x*a.p.y, y = b.v.y*a.v.y*b.p.x - a.v.y*b.v.x*b.p.y;
    if(a.v.y < 0) // We divided by this in the derivation so if < 0, flip the comparison
        std::swap(x, y);
    if(b.v.y < 0)
        std::swap(x, y);
    return x < y;
}

vec<ld> intersect(line<ll> r1, line<ll> r2) {
    // Returns the intersection of line r1 and r2 using Cramer's rule
    ll c1 = r2.p.x - r1.p.x, c2 = r2.p.y - r1.p.y;
    ll det = (r1.v.x*r2.v.y - r2.v.x*r1.v.y);
    ld t = ld(c1*r2.v.y - r2.v.x*c2)/det;

    return vec<ld> { (ld) r1.p.x, (ld) r1.p.y } + vec<ld> { (ld) r1.v.x, (ld) r1.v.y }*t;
}

int main() {
    ll ax1, ay1, ax2, ay2;
    ll bx1, by1, bx2, by2;

    std::cin >> ax1 >> ay1 >> ax2 >> ay2;
    std::cin >> bx1 >> by1 >> bx2 >> by2;

    vec<ll> a = vec<ll> { ax2, ay2 } - vec<ll> { ax1, ay1 };
    vec<ll> b = vec<ll> { bx2, by2 } - vec<ll> { bx1, by1 };

    line<ll> rs[4] = {
        { { ax1, ay1 }, { a.y, -a.x }, 0 },
        { { ax2, ay2 }, { a.y, -a.x }, 0 },
        { { bx1, by1 }, { b.y, -b.x }, 1 },
        { { bx2, by2 }, { b.y, -b.x }, 1 },
    };

    if(a.x*b.y - a.y*b.x == 0) { // If the given segments are parallel
        std::sort(rs, rs+4, f);
        if(rs[1].i != rs[0].i && f(rs[1], rs[2])) {
            std::cout << -1;
            return 0;
        }
        std::cout << 0;
        return 0;
    }

    // Otherwise, sort around the centroid and lace
    vec<ld> is[4] = { intersect(rs[0], rs[2]), intersect(rs[0], rs[3]),
                      intersect(rs[1], rs[2]), intersect(rs[1], rs[3]) };

    auto c = (is[0] + is[1] + is[2] + is[3])*0.25;

    std::sort(is, is+4, [c] (vec<ld> a, vec<ld> b) {
        return atan2(a.y-c.y, a.x-c.x) < atan2(b.y-c.y, b.x-c.x);
    });

    ld A = 0;
    for(int i = 0; i < 4; i++) {
        int j = (i+1)%4;
        A += is[i].x*is[j].y - is[i].y*is[j].x;
    }

    std::cout << std::fixed << std::setprecision(10) << 0.5*A;
}
