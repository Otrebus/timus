/* 1504. Good Manners - http://acm.timus.ru/problem.aspx?num=1504
 *
 * Strategy:
 * For each pair of cakes, find the isodistance line between them and the intersections where that
 * line crosses the edge of the table. Sort the intersections by their atan2 to the origin, and
 * initialize an array of cakes by their distance to the leftmost point of the table. Iterate over
 * the intersections around the table, and swap the corresponding cakes of each intersection in the
 * array while using the first cake in the array to update the position on the table to the closest
 * cake.
 *
 * Performance:
 * O(K^2), runs the tests in 0.171s using 23,752KB memory.
 */

#include <stdio.h>
#include <vector>
#include <cmath>
#include <numeric>
#include <algorithm>

using ld = long double;

struct vec {
    ld x, y;
    vec operator+ (const vec& p) const { return { p.x + x, p.y + y }; };
    vec operator- (const vec& p) const { return { p.x - x, p.y - y }; };
    vec operator/ (ld t) { return { x/t, y/t }; };
    vec operator* (ld t) { return { x*t, y*t }; };
    vec r() { return { y, -x }; } // A vector perpendicular to this
    ld d2() { return x*x + y*y; };
};

struct cake {
    vec v;
    short w, i;
} C[1000];

struct isec {
    ld t;
    short a, b;
};

ld R; int K;
short pos[1000]; // The cakes
std::vector<isec> v; // The intersection points

void calc(short i, short j) {
    // Finds the two intersection points of the isodistance line between pts i and j with the circle
    auto v1 = C[i].v, v2 = C[j].v;

    // The line
    auto p0 = (v1 + v2)/2;
    auto p1 = (v1 - v2).r();

    // The intersection points with the circle, set up the parametric for the line and and solve for
    // when its distance equals the radius
    auto c = (p0.d2() - R*R)/p1.d2();
    auto b = 2*(p0.x*p1.x + p0.y*p1.y)/p1.d2();

    auto s = std::sqrt(b*b/4 - c);
    auto t1 = -b/2 + s, t2 = -b/2 - s; // The intersections as the parameter along the line

    auto d1 = p0 + p1*t1, d2 = p0 + p1*t2; // Enter them into the line equation, get the atan2
    auto a1 = std::atan2(d1.y, d1.x), a2 = std::atan2(d2.y, d2.x);

    v.push_back( { a1, i, j } );
    v.push_back( { a2, i, j } );
}

int main() {
    scanf("%Lf %d", &R, &K);
    for(int i = 0; i < K; i++) {
        scanf("%Lf %Lf %hu", &C[i].v.x, &C[i].v.y, &C[i].w);
        C[i].i = i;
    }

    for(int i = 0; i < K; i++) // Find all the intersection points
        for(int j = i+1; j < K; j++)
            calc(i, j);

    // Sort the cakes by distance to the starting point
    vec c0 = { -R, -1e-9 }; // The starting point
    std::sort(C, C+K, [&c0] (const cake& a, const cake& b) {
        return (a.v - c0).d2() < (b.v - c0).d2();
    });
    for(int i = 0; i < K; i++)
        pos[C[i].i] = i;

    auto maxw = C[0].w;
    vec maxv = { -R, 0 };

    // Sort the intersections by atan
    std::sort(v.begin(), v.end(), [] (const auto& a, const auto& b) { return a.t < b.t; });

    for(auto p : v) { // Iterate over the intersections
        std::swap(C[pos[p.a]], C[pos[p.b]]); // Swap the corresponding cakes
        std::swap(pos[p.a], pos[p.b]);
        if(C[0].w > maxw) // Record the max
            maxw = C[0].w, maxv = { std::cos(p.t)*R, std::sin(p.t)*R };
    }
    printf("%.7Lf %.7Lf", maxv.x, maxv.y);
}
