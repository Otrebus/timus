/* 1703. Robotic Arm - http://acm.timus.ru/problem.aspx?num=1703
 *
 * Strategy:
 * Vector projection, law of cosines (see comments).
 *
 * Performance:
 * O(1), runs the tests in 0.015s using 184KB memory.
 */

#include <stdio.h>
#include <cmath>
#include <algorithm>

using ld = long double;
ld a, b;
const ld pi = std::acos(-1);

struct vec {
    ld x, y, z;
    ld operator*(const vec& v) { return x*v.x + y*v.y + z*v.z; }
    vec operator*(ld f) const { return { f*x, f*y, f*z }; }
    vec operator-(const vec& v) { return { x-v.x, y-v.y, z-v.z }; }
    vec operator+(const vec& v) { return { x+v.x, y+v.y, z+v.z }; }
    vec operator%(const vec& v) { return { y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x }; }
    ld d2() { return x*x + y*y + z*z; }
    ld d() { return std::sqrt(x*x + y*y + z*z); }
    vec n() { return { x/d(), y/d(), z/d() }; }
};
vec v0, v1;

ld angle(ld x) { // Returns acos(x) where x is clamped
    return std::acos(std::min(std::max(-1.0l, x), 1.0l));
}

int main() {
    scanf("%lf %lf %lf %lf %lf %lf %lf %lf", &a, &b, &v0.x, &v0.y, &v0.z, &v1.x, &v1.y, &v1.z);

    vec O { 0, 0, 0 };
    auto v = O - v0, w = v1 - v0;
    auto p = v0 + w*(v*w/w.d2()); // The grabbing point
    auto r = p - O;
    auto d = (O - p).d();
    
    // Cosines of the angles of the triangle
    ld t = angle((b*b - a*a - d*d)/(-2*a*d));
    ld s = angle((d*d - a*a - b*b)/(-2*a*b));
    
    // Check if the triangle is impossible to construct or the elbow angle violates the rules
    if(d > a + b || s < pi/2 - 1e-9)
        return printf("No solution."), 0;

    // Use the angles to reconstruct the coordinates
    auto q = (w%r).n(); // A unit vector perpendicular to r and the rail
    auto W = r.n()*a*cos(t) + q*a*sin(t);

    printf("%.20lf %.20lf %.20lf %.20lf", W.x, W.y, W.z, s);
}
