/* 1719. Kill the Shaitan-Boss - http://acm.timus.ru/problem.aspx?num=1719
 *
 * Strategy:
 * For every pair of lines A and B that we can form, move to some point p on line A and then to
 * the closest point (the projected one) on line B. Do ternary search over p to find the shortest
 * such a path.
 *
 * Performance:
 * O(log (max {|x|, |y|})), runs the tests in 0.015s using 304KB memory.
 */

#include <iostream>
#include <cmath>
#include <algorithm>
#include <iomanip>

using ld = long double;

struct vec {
    ld x, y;
    vec(ld x = 0, ld y = 0) : x(x), y(y) {}
    vec operator-(vec p) { return { x-p.x, y-p.y }; }
    vec operator+(vec p) { return { x+p.x, y+p.y }; }
    ld operator*(vec p) { return x*p.x + y*p.y; }
    vec operator*(ld f) { return { f*x, f*y }; }
    vec operator/(ld f) { return { x/f, y/f }; }
    ld d() { return std::sqrt(x*x + y*y); }
    ld d2() { return x*x + y*y; }
};

ld f(vec p, vec p1, vec p2) {
    // The distance to point p, and from p to the projected point on the line through p1 and p2
    auto v1 = p-p1, v2 = p2-p1;
    auto p3 = v2*(v1*v2)/(v2.d2()) + p1;
    return p.d() + (p-p3).d();
}

ld ternary(vec A, vec B, vec p1, vec p2) { // Ternary search
    vec L = p1 + (p2-p1)*-10000, R = p1 + (p2-p1)*10000;

    while((R - L).d() > 1e-8) {
        ld x = f(L + (R - L)/3, A, B), y = f(L + (R - L)*2.0L/3, A, B);
        if(x < y)
            R = L + (R - L)*2.0L/3;
        else
            L = L + (R - L)/3;
    }
    return f(L, A, B);
}

int main() {
    ld min = 1e100;
    vec v[4];
    for(int i = 0; i < 4; i++)
        std::cin >> v[i].x >> v[i].y;
    char i[4] = { 0, 1, 2, 3 };
    while(std::next_permutation(i, i+4)) // Try every permutation of lines
        min = std::min(min, ternary(v[i[0]], v[i[1]], v[i[2]], v[i[3]]));
    std::cout << std::setprecision(10) << min;
}
