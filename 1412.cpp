/* 1412. Autumn Tide - http://acm.timus.ru/problem.aspx?num=1412
 *
 * Strategy:
 * For each umbrella, calculate the amount of water it adds or removes from the amount that reaches
 * the field, which is equal to the area of the triangle formed by the center of the umbrella and
 * the intersection points with the sideline, either positively or negatively depending on what
 * side of the side line the center of the umbrella falls on.
 *
 * Performance:
 * O(N), runs the tests in 0.015s using an unreported amount of memory.
 */

#include <stdio.h>
#include <cmath>
#include <algorithm>

using ld = long double;

struct vec {
    ld x, y;
    ld operator* (const vec& p) const { return x*p.x + y*p.y; }
    vec operator- (const vec &p) const { return { x-p.x, y-p.y }; }
    vec operator+ (const vec &p) const { return { x+p.x, y+p.y}; }
    vec operator* (ld f) const { return { x*f, y*f }; }
    vec operator/ (ld f) const { return { x/f, y/f }; }
    ld operator% (const vec &p) const { return { x*p.y - y*p.x }; }
};

int W, L, N;
ld s;

bool inside(vec v) { return v.x <= L && v.x >= 0 && v.y <= W && v.y >= 0; }
ld sgn(ld x) { return (0 < x) - (x < 0); }

ld calc(vec p0, vec p1, vec c0) {
    // Calculates the intersection points of umbrella c0 with sideline (p0, p1), which side its
    // center falls on compared to the field, and the area to add or remove
    p0 = p0 - c0, p1 = p1 - c0;
    // Solve the quadratic equation |(p0+t*(p1-p0))| = 1 to get the intersection points
    auto d = (p1-p0)*(p1-p0), b = 2*(p0*(p1-p0))/d, c = (p0*p0 - 1)/d, D = b*b/4 - c;

    if(D >= 0) { // If the umbrella intersects the extended sideline
        auto t1 = -b/2 - std::sqrt(D), t2 = -b/2 + std::sqrt(D);
        auto r1 = p0 + (p1-p0)*t1, r2 = p0 + (p1-p0)*t2;
        if(inside(r1+c0) && inside(r2+c0)) // If points are on the sideline
            return sgn((p1-p0)%(p0))*std::abs(r1%r2)/2; // Add or subtract depending on the center
    }
    return 0;
}

void calc(vec v) {
    // Adds/removes the area of rain added or removed by the umbrella at center v
    vec p[4] = {{ 0, 0 }, { 0, W }, { L, W }, { L, 0 }};
    for(int i = 0; i < 4; i++)
        s += calc(p[i], p[(i+1)%4], v);
}

int main() {
    scanf("%d %d %d", &W, &L, &N);

    for(int i = 0; i < N; i++) {
        vec v;
        scanf("%lf %lf", &v.x, &v.y);
        calc(v);
    }
    printf("%.20lf", W*L-s);
}
