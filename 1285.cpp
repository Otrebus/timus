/* 1285. Thread in a Hyperspace - http://acm.timus.ru/problem.aspx?num=1285
 *
 * Strategy:
 * This problem is solved in the same way as #1075, but where we changed the way we calculate
 * perpendicular vectors "in the general direction" of some vector by using vector projection
 * instead of a cross products (see function perp below).
 *
 * Performance:
 * O(1), runs the tests in 0.015s using 196KB memory.
 */

#include <stdio.h>
#include <cmath>
#include <algorithm>

using ld = long double;

const int N = 8;

struct vec { // Standard vector stuff
    ld c[N];

    vec operator-(const vec& v) const {
        vec ret;
        for(int i = 0; i < N; i++)
            ret.c[i] = c[i]-v.c[i];
        return ret;
    }
    vec operator+(const vec& v) const {
        vec ret;
        for(int i = 0; i < N; i++)
            ret.c[i] = c[i]+v.c[i];
        return ret;
    }
    ld operator*(vec& v) {
        ld ret = 0;
        for(int i = 0; i < N; i++)
            ret += c[i]*v.c[i];
        return ret;
    }
    vec operator*(ld f) {
        vec ret;
        for(int i = 0; i < N; i++)
            ret.c[i] = c[i]*f;
        return ret;
    }
    ld d2() {
        ld r = 0;
        for(int i = 0; i < N; i++)
            r += c[i]*c[i];
        return r;
    }
    ld d() {
        ld r = 0;
        for(int i = 0; i < N; i++)
            r += c[i]*c[i];
        return std::sqrt(r);
    }
    vec n() {
        vec ret;
        ld f = d();
        for(int i = 0; i < N; i++)
            ret.c[i] = c[i]/f;
        return ret;
    }
};

vec A, B, C;
ld R;

ld angle(vec a, vec b) {
    // Returns the angle between vectors a and b
    return std::acos(std::min(1.L, std::max(-1.L, a*b/(a.d()*b.d()))));
}

vec perp(vec v1, vec v2) {
    // Returns a perpendicular vector to v1 in the plane (and general direction) of v2
    if(v2.d() < 1e-7)
        for(int i = 0; i < N; i++)
            v2.c[i] = 1001+i;
    return (v2 - v1*(v1*v2)*(1/v1.d2())).n();
}

int main() {
    for(int i = 0; i < N; i++)
        scanf("%lf", &A.c[i]);

    for(int i = 0; i < N; i++)
        scanf("%lf", &B.c[i]);

    for(int i = 0; i < N; i++)
        scanf("%lf", &C.c[i]);

    scanf("%lf", &R);

    vec v1 = B-A, v2 = C-A;
    ld t = (v1*v2)/v1.d(); // The parameter along B-A where the center of the sphere projects to
    vec vp = (v1.n()*t - v2); // The line from C to its closest point on B-A
    ld dvp = vp.d();
    if(t != t || t <= 0 || t >= v1.d() || dvp >= R) // A = B, or line runs outside sphere
        return printf("%.10lf", v1.d()), 0;

    ld d = (A-C).d(), d2 = (A-C).d2();
    auto vd = perp(A-C, vp); // A perpendicular line to A-C in the general direction of vp

    // Since A-B and vp form a right angle, we have R/d and sqrt(d*d-R*R)/d be the cosine and sine
    // of a triangle where d is the distance from A to the tangent point. We can then use those
    // values together with vd to find the tangent point p1
    auto p1 = C + (vd*std::sqrt(d2-R*R) + (A-C).n()*R)*(R/d);
    d = (B-C).d(), d2 = (B-C).d2();

    // Same for the point B, for p2
    vd = perp(B-C, vp);
    auto p2 = C + (vd*std::sqrt(d2-R*R) + (B-C).n()*R)*(R/d);

    ld ans = (p1-A).d() + (p2-B).d() + R*angle(p1-C, p2-C);
    printf("%.10lf", ans);
}
