/* 1436. Billboard. Version 2 - http://acm.timus.ru/problem.aspx?num=1436
 *
 * Strategy:
 * Calculate the intersection of the extended line segment (billboard) with the abscissa and do a
 * ternary search over the viewing angle to the left and right from that point.
 *
 * Performance:
 * Logarithmic in the coordinate magnitude, runs the tests in 0.015s using 272KB memory.
 */

#include <iostream>
#include <cmath>
#include <iomanip>

using ld = long double;

long double xa, ya, xb, yb;

ld f(ld x) {
    // Calculates the viewing angle of the billboard from point x on the road
    ld x1 = xa - x, y1 = ya, x2 = xb - x, y2 = yb;
    auto m = (x1*x2 + y1*y2)/(std::sqrt(x1*x1+y1*y1)*std::sqrt(x2*x2+y2*y2));
    return std::acos(std::max(m, -1.0l));
}

ld ts(ld l, ld r) {
    // Ternary search between l and r on the x-axis
    while(std::abs(l - r) > 1e-9) {
        ld x1 = l + (r - l)/3.0, x2 = l + 2.0l*(r - l)/3.0;
        if(f(x2) > f(x1)) l = x1; else r = x2;
    }
    return f(r);
}

ld solve() {
    if(!ya || !yb) // Special case - one edge of the billboard touches the road, count angle as pi
        return std::acos(-1);
    if(ya == yb) // If board is parallel with road
        return f((xb + xa)/2);

    ld dy = yb-ya, dx = xb-xa, k = dy/dx, x = xa == xb ? xa : (xa*k - ya)/k;
    return std::max(ts(x, x+1e7), ts(x-1e7, x)); // Ternary search both sides of the intersection
}

int main() {
    std::cin >> xa >> ya >> xb >> yb;
    std::cout << std::fixed << std::setprecision(6) << solve();
}
