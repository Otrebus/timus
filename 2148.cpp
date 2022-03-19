/* 2148. Insane Shot - http://acm.timus.ru/problem.aspx?num=2148
 *
 * Strategy:
 * This solution solves the more general problem of having the two distances possibly be unequal,
 * by calculating the angles between some arbitrary point on the edge of the tank with the
 * ingoing/outgoing vectors and performing binary search over that point until the angles are equal.
 *
 * Performance:
 * Runs the tests in 0.031s using 260KB memory.
 */

#include <iostream>
#include <cmath>

using ld = long double;

struct vec {
    ld x, y;
    vec operator-(const vec& v) const { return { x-v.x, y-v.y }; }
    vec operator+(const vec& v) const { return { x+v.x, y+v.y }; }
    ld operator*(vec& v) { return x*v.x + y*v.y; }
    vec operator/(ld f) { return { x/f, y/f }; }
    vec operator*(ld f) { return { x*f, y*f }; }
    ld d2() { return x*x + y*y; }
    ld d() { return std::sqrt(d2()); }
    vec n() { return *this/d(); }
};

vec A, B, C;
ld R;

ld calc(vec m) {
    // Calculates the difference between the incoming and outgoing angles given some point m
    // on the tank's rim
    auto r = (m-C).n();
    auto x = C + r*R;
    auto v2 = (B-x).n();
    auto v1 = (A-x).n();
    return v2*r - v1*r;
}

vec solve() {
    // Solves the problem using binary search over a point on the tank, with the vectors
    // towards the two other given points in the problem as boundaries
    auto l = C + (A-C).n(), r = C + (B-C).n();
    while((r-l).d() > 1e-9) {
        auto m = l + (r-l)/2;
        auto d = calc(m);
        if(d < 0)
            l = m;
        else
            r = m;
    }
    return C+(l-C).n()*R;
}

int main() {
    scanf("%Lf %Lf %Lf %Lf %Lf %Lf %Lf", &C.x, &C.y, &R, &A.x, &A.y, &B.x, &B.y);
    auto v1 = B-A, v2 = C-A;
    auto p = A + v1*(v1*v2)/v1.d2();
    if((p-C).d() < R + 1e-9)
        return std::cout << "No way", 0;
    auto a = solve();
    printf("%.10Lf  %.10Lf", a.x, a.y);
}
