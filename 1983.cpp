/* 1983. Nectar Gathering - http://acm.timus.ru/problem.aspx?num=1983
 *
 * Strategy:
 * Clip the triangle to the sphere (similarly to Sutherland-Hodgman), and calculate the area of the
 * resulting polygon and circular segments.
 *
 * Performance:
 * O(1), runs the tests in 0.031s using an unreported amount of memory.
 */

#include <stdio.h>
#include <vector>
#include <cmath>
#include <iostream>

using ld = long double;
const ld R2 = 10000;
const ld R = 100;
const ld pi = 3.14159265358979323846264338;

enum Type { Inside, Outside, Entering, Leaving };

struct vec { // Standard 3d vector stuff for the most part
    ld x, y, z;
    Type type;
    vec(vec v, Type t) { *this = v, type = t ; }
    vec(ld x = 0, ld y = 0, ld z = 0) : x(x), y(y), z(z) { }
    ld operator* (const vec& p) const { return x*p.x + y*p.y + z*p.z; }
    vec operator- (const vec &p) const { return { x-p.x, y-p.y, z-p.z }; }
    vec operator+ (const vec &p) const { return { x+p.x, y+p.y, z+p.z }; }
    vec operator* (ld f) const { return { x*f, y*f, z*f }; }
    vec operator% (const vec &p) const { return { y*p.z - z*p.y, z*p.x - x*p.z, x*p.y - y*p.x }; }
    ld d2() const { return x*x + y*y + z*z; }
    ld d() const { return std::sqrt(x*x + y*y + z*z); }
} P[3];

ld clamp(ld a) { return std::min(std::max(-1.l, a), 1.l); } // Clamps a between [-1, 1]

ld slice(vec p0, vec p1) {
    // Returns the area of the pie slice produced by vectors p0 and p1
    return std::acos(clamp((p1*p0)/(p1.d()*p0.d())))*p1.d2()/2;
}

int main() {
    for(int i = 0; i < 3; i++) {
        scanf("%lf %lf %lf", &P[i].x, &P[i].y, &P[i].z);
        P[i].type = P[i].d2() > R2 ? Outside : Inside;
    }
    ld A = 0;
    vec n = (P[2]-P[0])%(P[1]-P[0]); // The normal of the triangle
    std::vector<vec> V; // The vertices of the polygon we clip the triangle into

    for(int i = 0; i < 3; i++) {
        // Calculate the intersection between the given line segment and sphere
        // by solving |(p0+t(p1-p0))|^2 = R^2 
        auto p0 = P[i], p1 = P[(i+1)%3];
        auto d = (p1-p0)*(p1-p0), b = 2*(p0*p1 - p0*p0)/d, c = (p0*p0 - R2)/d, D = b*b/4 - c;

        if(p0.type == Inside)
            V.push_back(p0);

        if(D >= 0) {
            auto t1 = -b/2 - std::sqrt(D), t2 = -b/2 + std::sqrt(D);
            if(t1 >= 0 && t1 <= 1) // First intersection point
                V.emplace_back(p0 + (p1-p0)*t1, Outside ? Entering : Leaving);
            if(t2 >= 0 && t2 <= 1) // Second intersection point
                V.emplace_back(p0 + (p1-p0)*t2, Leaving);
        }
    }

    // Calculate the closest point from the sphere center to the plane of the triangle, by solving
    // n*u = p0 + (p2-p0)*v + (p1-p0)*w using Cramer's rule
    vec v0 = n, v1 = (P[0]-P[1]), v2 = (P[0]-P[2]), d = P[0];
    ld det = v0*(v1%v2), u = d*(v1%v2)/det, v = v0*(d%v2)/det, w = v0*(v1%d)/det;

    // Look for places where the sphere clips the triangle
    for(int i = 0; i < V.size(); i++) {
        vec c = n*u;
        vec p1 = V[(i+1)%V.size()], p0 = V[i];
        if(p0.type == Leaving && p1.type == Entering) {
            // Calculate the area of the circle segment that we wrap around
            vec r = n%(p1-p0);
            p1 = p1 - c, p0 = p0 - c;
            auto pie = slice(p1, p0), tri = (p1%p0).d()/2;
            A += (p1*(r-c) < 0) ? pi*p1.d2() - pie + tri : pie - tri;
        }
    }
    if(!V.size()) {
        // No point intersected with the sphere, check if it pops through the triangle
        if(v > 0 && v < 1 && w > 0 && w < 1 && v + w < 1) {
            auto a = P[0] + (P[1]-P[0])*v + (P[2]-P[0])*w;
            auto r = std::sqrt(std::max(R2 - a.d2(), 0.l));
            printf("%.10lf", r*r*pi);
        } else
            printf("0");
    } else {
        // Add the area of the polygon resulting from the clipping of the triangle
        for(int i = 1; i < int(V.size())-1; i++)
            A += 0.5*((V[i]-V[0])%(V[i+1]-V[0])).d();
        printf("%.10lf", A);
    }
}
