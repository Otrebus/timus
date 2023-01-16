/* 1482. Triangle Game - http://acm.timus.ru/problem.aspx?num=1482
 *
 * Strategy:
 * Try the following for every permutation of the vertices of the second triangle:
 * Pick the first vertex from the first and second triangles and check if the remaining vertices
 * are separated by the same vector to check for the translation transformation. For checking a
 * rotation, we pick two edges of the given triangles and check the angle between them (this is
 * the rotation angle) and deduce the pivot point which exists as the far vertex in a symmetric
 * triangle with the other two vertices belonging to one each of edges we checked, and where the
 * angle of the pivot point vertex is half the rotation angle. We then rotate the first triangle
 * with the given angle and check if it fits the second.
 *
 * Performance:
 * O(1), runs the tests in 0.015s using 64KB.
 */

#include <stdio.h>
#include <cmath>
#include <algorithm>

using ld = long double;
const ld eps = 1.0e-6;
const ld pi = std::acos(-1);

struct point {
    ld x, y;
    point operator-() const { return { -x, -y }; };
    point operator+ (const point& a) const { return { x+a.x, y+a.y }; }
    point operator- (const point& a) const { return { x-a.x, y-a.y }; }
    ld operator* (const point& a) const { return x*a.x + y*a.y; }
    ld operator% (const point& a) const { return x*a.y - y*a.x; }
    point operator/ (ld f) const { return { x/f, y/f }; }
    point operator* (ld f) const { return { x*f, y*f }; }
    operator bool() { return x || y; }
    bool operator== (point v) { return (*this -v).d() < eps; }
    point n() { return (*this)/std::sqrt(x*x+y*y); }
    point rot(ld a) { return { std::cos(a)*x  - std::sin(a)*y, std::sin(a)*x + std::cos(a)*y }; }
    ld d() { return std::sqrt(x*x+y*y); }
    point r() { return { y, -x }; }
};

bool solve(point v1, point v2, point v3, point w1, point w2, point w3) {
    // Check translation
    auto T = v1 - w1;
    if(v1 - T == w1 && v2 - T == w2 && v3 - T == w3)
        return printf("1 %.15lf %.15lf", T.x, T.y);

    // Check rotation
    auto u = (w2-w1).n(), v = (v2-v1).n(); // The two edges
    ld a = std::acos(u*v); // Their angles
    // A vector perpendicular to the line between them
    auto p = T ? (u%v < 0 ? T.r() : -T.r()).n() : point { 0.0l, 0.0l };
    // The pivot point
    auto o = w1 + T/2.0l + p*(T.d()/2/std::tan(a/2));
    
    if(u%v < 0) // Reverse the angle depending on rotation
        a = 2*pi-a;

    // Check of the rotation fits
    w1 = (w1-o).rot(a)+o, w2 = (w2-o).rot(a)+o, w3 = (w3-o).rot(a)+o;
    if(w1 == v1 && w2 == v2 && w3 == v3)
        return printf("2 %.15lf %.15lf %.15lf", o.x, o.y, 180*a/pi);
    return false;
}

int main() {
    point v[6];
    for(int i = 0; i < 6; i++)
        scanf("%lf %lf", &v[i].x, &v[i].y);

    // Check if any of the permutation of the vertices work out
    for(int i = 0; i < 3; i++)
        if(solve(v[0], v[1], v[2], v[3+i%3], v[3+(i+1)%3], v[3+(i+2)%3]) ||
           solve(v[0], v[1], v[2], v[3+i%3], v[3+(i+2)%3], v[3+(i+1)%3]))
            return 0;

    printf("0");
}