/* 1909. Space Recon - http://acm.timus.ru/problem.aspx?num=1909
 *
 * Strategy:
 * Form a coordinate system with the velocity as the z-axis and and one axis perpendicular to
 * the velocity vector and the vector between the probes, calculate the intersection of the
 * projection of the planet onto the xy-plane with the x axis, and also taking into account
 * the starting points of the probes points and the vector between them in relation to the
 * curvature of the planet. See comments.
 *
 * Performance:
 * O(1), runs the tests in 0.031s using 156KB memory.
 */

#include <stdio.h>
#include <cmath>
#include <algorithm>

using ld = long double;

struct vec {
    ld x, y, z;
    vec operator-(const vec& v) const { return { x-v.x, y-v.y, z-v.z }; }
    vec operator+(const vec& v) const { return { x+v.x, y+v.y, z+v.z }; }
    ld operator*(const vec& v) { return x*v.x + y*v.y + z*v.z; }
    vec operator*(ld f) { return { x*f, y*f, z*f }; }
    vec operator/(ld f) { return { x/f, y/f, z/f }; }
    vec operator%(vec v) { return { y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x }; }
    ld d() { return std::sqrt(x*x + y*y + z*z); }
    ld d2() { return x*x + y*y + z*z; }
    vec n() { return { x/d(), y/d(), z/d() }; }
};

vec a, b, c, p1, p2, v;
ld R;
ld eps = 1e-9;

ld dv(ld x, ld r) { // The derivative of the height of a halfsphere
    return x/sqrt(r*r - x*x);
}

ld find(ld r, ld k) { // Binary search
    ld L = -r, R = r;
    while(R - L > 1e-9) {
        ld m = L + (R - L)/2;
        if(dv(m, r) < k)
            L = m;
        else
            R = m;
    }
    return R;
}

int main() {
    scanf("%lf %lf %lf %lf", &c.x, &c.y, &c.z, &R);
    scanf("%lf %lf %lf", &p1.x, &p1.y, &p1.z);
    scanf("%lf %lf %lf", &p2.x, &p2.y, &p2.z);
    scanf("%lf %lf %lf", &v.x, &v.y, &v.z);

    auto w = p2 - p1;
    vec a{0, 0, 0}, b{0, 0, 0};
    ld cx, cy, cz;
    v = v.n();
    bool along = false;
    if((v%w.n()).d() < 1e-5) { // The velocity vector is parallel with the vector between the probes
        w = vec { 1000001, 1000002, 1000003 };
        along = true;
    }

    // Form axes perpendicular to the velocity and the vector between the probes
    a = (w%v).n();
    b = (v%a).n();

    ld x = w*b, y = v*w; // The coordinates of the second probe in the new system
    // Project the position of the planet onto the new coordinate system
    cy = (c-p1)*a, cx = (c-p1)*b, cz = (c-p1)*v;
    ld d = ((w%v)%w)*(c-p1);
    auto D = R*R - cy*cy; // The discriminant when solving for when the circle meets the new x-axis
    if(D < -eps) // Planet doesn't intersect the x axis
        return printf("False alarm"), 0;
    if(along) {
        eps = 1e-5;
        if(d > 0 && cx*cx + cy*cy <= R*R+eps || (cx-x)*(cx-x) + cy*cy <= R*R+eps)
            return printf("Crash"), 0;
    }

    if(!along) {
        D = std::max(D, 0.l);
        // The points of intersection of the sphere with the x axis
        auto x1 = cx - std::sqrt(D);
        auto x2 = cx + std::sqrt(D);

        // Find the point where the planet first touches the line between the probes and see if it
        // falls between the probes, and in the right direction
        ld p = find(std::sqrt(D), y/x);
        if(p+cx > eps && p+cx < x-eps && (c - (p1 + w.n()*(p+cx)/(w*b/w.d())))*v > -eps)
            return printf("Warning"), 0;

        // If we're heading towards the planet and the intersections with the x-axis falls outside
        // the range of the probes, we crash into it
        if(v*(c-p1) > -eps && x1 < eps && x2 > -eps || v*(c-p2) > -eps && x1 < x+eps && x2 > x-eps)
            return printf("Crash"), 0;
    }
    printf("False alarm");
}
