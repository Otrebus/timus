/* 1093. Darts - http://acm.timus.ru/problem.aspx?num=1093
 *
 * Strategy:
 * Find the intersection of the path of the dart M(t) with the plane of the dart board by solving
 * n*(M(t)-c) = 0 for t = t1, t2 and then check that the intersection point is inside the board,
 * meaning |M(t_i)-c| < r for i = 1 or 2.
 *
 * Performance:
 * O(1), runs the tests in 0.015s using 268KB memory.
 */

#include <iostream>
#include <cmath>

using ld = long double;
ld cx, cy, cz, nx, ny, nz, r, sx, sy, sz, vx, vy, vz;
const ld eps = 1e-9;

ld dist(ld t) { // Returns |M(t)-c|
    auto sq = [] (auto t) { return t*t; };
    return std::sqrt(sq(sx+vx*t-cx)+sq(sy+vy*t-cy)+sq(sz+vz*t-5*t*t-cz));
}

bool solve() {
    // Set up n*v(t) = n*c and solve the resulting quadratic equation
    ld a = -5*nz, b = nx*vx + ny*vy + nz*vz, c = nx*sx + ny*sy + nz*sz - nx*cx - ny*cy - nz*cz;
    ld D = 4*a*c, Q = std::sqrt(b*b - D), t1, t2;

    if(D > b*b + eps) // Parabola falls short of the plane, or is parallel to it
        return false;

    if(!a) { // Plane is vertical
        t1 = -c/b, t2 = -1; // (Parabola can be parallel to the plane (b = 0); then t1 = -inf)
    } else
        t1 = (-b + Q)/(2*a), t2 = (-b - Q)/(2*a);

    return t1 > 0 && dist(t1) < r - eps || t2 > 0 && dist(t2) < r - eps;
}

int main() {
    std::cin >> cx >> cy >> cz >> nx >> ny >> nz >> r >> sx >> sy >> sz >> vx >> vy >> vz;
    std::cout << (solve() ? "HIT" : "MISSED");
}
