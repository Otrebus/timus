/* 1256. Cemetery Guard - http://acm.timus.ru/problem.aspx?num=1256
 *
 * Strategy:
 * Two subsequent applications of ternary search, each corresponding to the point on the implied
 * circles around R2 and R3 that we place the food, where the searches are bounded by the triangle
 * (R1, R2, R3).
 *
 * Performance:
 * O(1), runs the tests in 0.015s using an unreported amount of memory.
 */

#include <stdio.h>
#include <cmath>
#include <algorithm>

using ld = long double;

struct vec {
    ld x, y;
    vec operator- (const vec& v) { return { x-v.x, y-v.y }; }
    vec operator* (const ld f) { return { x*f, y*f }; }
    vec operator+ (const vec& v) { return { x+v.x, y+v.y }; }
    ld d() { return std::sqrt(x*x + y*y); }
    vec n() { return { x/d(), y/d() }; }
};

vec p0, p1, p2;
ld R1, R2, R3, R4;

ld f2(ld t, vec w) {
    // Similar to f1 below; calculates the total distance if we drop the second plate of food at
    // position v described below, and fed the first dog at vector w
    vec v1 = p0-p2, v2 = w-p2;
    vec v = p2 + (v1*t + v2*(1-t)).n()*R4;

    return (p0-v).d() + (v-w).d() + (p0-w).d();
}

ld t2(vec v) {
    // Does the second ternary search for the smallest distance given that we drop the first plate
    // at position v
    ld L = 0, R = 1, a, b;
    while(R - L > 1e-9) {
        auto p1 = L + (R-L)*(1.0/3), p2 = L + (R-L)*(2.0/3);
        a = f2(p1, v), b = f2(p2, v);
        if(a > b)
            L = p1;
        else
            R = p2;
    }
    return a;
}

ld f1(ld t) {
    // Returns the smallest answer if we drop the first plate of food at position v described below
    vec v1 = p0-p1, v2 = p2-p1;
    vec v = p1 + (v1*t + v2*(1-t)).n()*R4;

    return t2(p1);
}

ld t1() {
    // Carries out the first ternary search for where to drop the first plate
    ld L = 0, R = 1, a, b;
    while(R - L > 1e-9) {
        auto p1 = L + (R-L)*(1.0/3), p2 = L + (R-L)*(2.0/3);
        a = f1(p1), b = f1(p2);
        if(a > b)
            L = p1;
        else
            R = p2;
    }
    return a;
}

int main() {
    scanf("%lf %lf %lf %lf", &R1, &R2, &R3, &R4);
    if(R1 > R2)
        std::swap(R1, R2);
    if(R1 < R4) // Special case when R1 is inside one of the radiuses
        return printf("%lf", 2*std::max(R2-R4, 0.l)), 0;

    ld x = (R1*R1+R2*R2-R3*R3)/(2*R1); // Use the law of cosines to deduce how the triangle looks
    p0 = { 0, 0 }, p1 = { R1, 0 }, p2 = { x, std::sqrt(R2*R2-x*x) };

    printf("%lf", t1());
}
