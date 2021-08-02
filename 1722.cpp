/* 1722. Observation Deck - http://acm.timus.ru/problem.aspx?num=1722
*
 * Strategy:
 * The area in question is the part of the city that is bounded by the outer tangent lines of the
 * two towers, from their intersection with the city limit (or each other on the far side of the
 * new tower), to the tangent points touching the new tower, and the circle segment between those
 * tangent points that is facing us. We can divide this area thusly:
 * 
 *   1) For the case when the outer tangents intersect inside the city limits, the area is
 *      a circle segment of the new tower facing us, plus two triangles with one vertex
 *      in the center of the new tower, to a tangent point, to the intersection point.
 *
 *   2) For the case when the outer tangents do not intersect inside the city limits, the
 *      area is the mentioned circle segment, plus two triangles formed by the center of
 *      the new tower, the tangent point and the intersection of the tangent with
 *      the city limits, plus the triangle formed by the intersections between the tangents
 *      and the city limits and the center of the new tower, plus the chord formed by those
 *      intersections and the city limit.
 *
 * Performance:
 * O(1), runs the tests in 0.015s, with the judge not reporting the memory usage (site bugged?).
 */

#include <iostream>
#include <cmath>
#include <iomanip>

using ld = long double;
const ld pi = 3.141592653589793238462643383279;

struct vec {
    ld x, y;

    vec operator-(const vec& v) const { return { x-v.x, y-v.y }; }
    vec operator+(const vec& v) const { return { x+v.x, y+v.y }; }
    vec operator-() const { return { -x, -y }; }
    ld operator*(const vec& v) const { return x*v.x + y*v.y; } // Dot product
    ld d2() const { return x*x + y*y; } // Length squared
    ld d() const { return std::sqrt(x*x + y*y); }
    vec operator*(const ld c) const { return { c*x, c*y }; }
    vec operator/(const ld c) const { return { x/c, y/c }; }
    vec n() const { return vec { x, y }/std::sqrt(x*x + y*y); }; // Returns the normalized vector
};

ld area(vec v1, vec v2, vec v3) { // Area of a triangle of the given vertices
    return 0.5*(v1.x*v2.y + v2.x*v3.y + v3.x*v1.y - v2.x*v1.y - v3.x*v2.y - v1.x*v3.y);
}

int main() {
    ld R, r1, r2, x, y;
    std::cin >> R >> r1 >> r2;
    std::cin >> x >> y;

    vec c1{ 0, 0 }, c2{ x, y };

    bool swapped = false;
    // We assume in the calculations that r1 is the larger circle. The tangents are set up for
    // calculation by forming a rectangle between the the center of r2, r2's tangent point, r1's
    // tangent point, and the last vertex being a distance of r1-r2 between r2's tangent point and
    // r2's center. We then also have a triangle between the centers of r1 and r2 and the last
    // vertex. Using a little bit of trigonometry we can then calculate the tangent points.
    if(r2 > r1)
        std::swap(r1, r2), std::swap(c1, c2), swapped = true;

    vec c = c2 - c1, cr = { c.y, -c.x }; // C points from c1 to c2, vr is perpendicular
    // Using c and cr as basis vectors and w1 and w2 as cos and sin below ...
    vec w1 = c*(r1-r2)/c.d2(), w2 = cr*std::sqrt((c.d2() - (r1-r2)*(r1-r2)))/c.d2();
    vec p1 = w1 + w2, p2 = w1 - w2;

    // ... we can use them to find the tangent points both for r2 and r1 since there's similarity
    vec v0 = c1 + p1*r1, v1 = c2 + p1*r2, v2 = c1 + p2*r1, v3 = c2 + p2*r2;

    if(swapped)
        std::swap(v0, v3), std::swap(v1, v2), std::swap(c1, c2), std::swap(r1, r2);

    // Find the intersection of the tangents using Cramer's rule
    vec d1 = v1-v0, d2 = v3-v2, b = v2-v0;

    ld det = d1.x*-d2.y + d2.x*d1.y;
    ld s = (b.x*-d2.y + d2.x*b.y)/det, t = (d1.x*b.y - b.x*d1.y)/det;

    vec P = v0 + (v1-v0)*s;
    ld A;
    if(r2 >= r1 || P.d() > R) { // If the intersection is outside the city limits
        // Find the intersection between the tangent points and the city limits circle; this is a
        // second degree equation (the distance between the points of the parametric representation
        // of the points on the line and the origin should be equal to the radius of the city)
        ld a = (v1-v0)*(v1-v0);
        ld c = ((v0-c1)*(v0-c1) - R*R)/a;
        ld b = (v1-v0)*(v0-c1)*2/a;
        ld s = -b/2 + std::sqrt(b*b/4 - c);
        vec u1 = v0 + (v1-v0)*s, u2 = v2 + (v3-v2)*s; // u1 and u2 are the intersections
        ld th = 2*std::acos(w1.d()); // This is the angle in the tower circle segment facing us
        if(r1 > r2) // If the second tower is smaller, adjust the slice
            th = 2*pi - th;
        ld slice = th/2*r2*r2;
        ld a1 = area(v3, c2, u2); // Leftmost triangle
        ld a2 = area(c2, v1, u1); // Rightmost triangle (should be equal to the left cuz symmetry)
        ld a3 = area(c2, u1, u2); // Middle triangle
        ld chord = (std::acos(u1.n()*u2.n())/(2*pi))*pi*R*R - area({ 0, 0 }, u1, u2);
        A = slice + a1 + a2 + a3 + chord;

    } else { // Intersection inside city limits
        vec u1 = v1-c2, u2 = v3-c2;
        ld pie = (2*pi - std::acos(u1.n()*u2.n()))*r2*r2/2; // The circle segment facing us
        ld t1 = area(c2, v1, P), t2 = area(c2, P, v3); // The two triangles beyond it
        A = pie + t1 + t2;
    }

    std::cout << std::fixed << std::setprecision(10) << 100*A/(pi*R*R);
}
