/* 1075. Thread in a Space - http://acm.timus.ru/problem.aspx?num=1075
 *
 * Strategy:
 * We check for two cases. If the thread runs completely outside the sphere, the answer is simply
 * the length of the thread. If the line defined by the thread intersects the sphere, the length
 * is the distance from point A to the tangent point of A and the sphere, on the side of the center
 * of the sphere the line runs, plus the length of the arc between that point and the tangent point
 * of B and the sphere, plus the distance from that point to B.
 *
 * Performance:
 * O(1), runs the tests in 0.031s using an 172KB memory.
 */

#include <stdio.h>
#include <cmath>
#include <algorithm>

using ld = long double;

struct vec {
    ld x, y, z;
    vec operator-(const vec& v) const { return { x-v.x, y-v.y, z-v.z }; }
    vec operator+(const vec& v) const { return { x+v.x, y+v.y, z+v.z }; }
    ld operator*(vec& v) { return x*v.x + y*v.y + z*v.z; }
    vec operator*(ld f) { return { x*f, y*f, z*f }; }
    vec operator/(ld f) { return { x/f, y/f, z/f }; }
    vec operator%(vec v) { return { y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x }; }
    ld d() { return std::sqrt(x*x + y*y + z*z); }
    ld d2() { return x*x + y*y + z*z; }
    vec n() { return { x/d(), y/d(), z/d() }; }
};

vec A, B, C;
ld R;

ld angle(vec a, vec b) {
    // Returns the angle between vectors a and b
    return std::acos(std::min(1.L, std::max(-1.L, a*b/(a.d()*b.d()))));
}

vec perp(vec v1, vec v2) {
    // Returns a perpendicular vector to v1 in the plane (and general direction) of v2
    if(!(v1%v2).d())
        v2 = { 1001, 1002, 1003 };
    return ((v1%v2)%(v1)).n();
}

int main() {
    scanf("%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf",
          &A.x, &A.y, &A.z, &B.x, &B.y, &B.z, &C.x, &C.y, &C.z, &R);

    vec v1 = B-A, v2 = C-A;
    ld t = (v1*v2)/v1.d(); // The parameter along B-A where the center of the sphere projects to
    vec vp = (v1.n()*t - v2); // The line from C to its closest point on B-A
    ld dvp = vp.d();
    if(t != t || t <= 0 || t >= v1.d() || dvp >= R) { // A = B, or line runs outside sphere
        return printf("%.10lf", v1.d()), 0;
    }

    ld d = (A-C).d(), d2 = (A-C).d2();
    auto vd = perp(A-C, vp); // A perpendicular line to A-C in the general direction of vp

    // Since A-B and vp form a right angle, we have R/d and sqrt(d*d-R*R)/d be the cosine and sine
    // of a triangle where d is the distance from A to the tangent point. We can then use those
    // values together with vd to find the tangent point p1
    auto p1 = C + (vd*std::sqrt(d2-R*R) + (A-C).n()*R)*R/d;
    d = (B-C).d(), d2 = (B-C).d2();

    // Same for the point B, for p2
    vd = perp(B-C, vp);
    auto p2 = C + (vd*std::sqrt(d2-R*R) + (B-C).n()*R)*R/d;

    ld ans = (p1-A).d() + (p2-B).d() + R*angle(p1-C, p2-C); // The sum described in the header
    printf("%.10lf", ans);
}
