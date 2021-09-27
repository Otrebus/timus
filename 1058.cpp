/* 1058. Chocolate - http://acm.timus.ru/problem.aspx?num=1058
 *
 * Strategy:
 * For each edge, do a ternary search over the points on the edge to find the shortest cutting
 * line produced from cutting evenly from such a point, where such a cutting line is found by doing
 * a binary search over the rest of the polygon from that point.
 *
 * Performance:
 * O(N^2), runs the tests in 0.015s using 348KB memory
 */

#include <iostream>
#include <algorithm>
#include <cmath>

using ld = long double;

struct vec {
    ld x, y;
    vec operator-(const vec& v) { return { x-v.x, y-v.y }; }
    vec operator+(const vec& v) { return { x+v.x, y+v.y }; }
    vec operator*(ld d) { return { x*d, y*d }; }
    ld d() { return std::sqrt(x*x+y*y); }
} v[201];

int N;          // Number of vertices of the polygon
ld L;           // Total length of the polygon
ld A[201][201]; // The area of the polygon between points i to j
ld B[201];      // The length of each edge

ld preArea(int a, int b) {
    // Calculates the area of the polygon between vertices with indices a and b
    ld r = 0;
    for(int i = 0; i <= b-a; i++)
        r += v[a+i%(1+b-a)].x*v[a+(i+1)%(1+b-a)].y - v[a+(i+1)%(1+b-a)].x*v[a+i%(1+b-a)].y;
    return r*0.5;
}

ld area(vec a, vec b, vec c, vec d) {
    // The area of a quadrilateral with the given points
    return 0.5*(a.x*b.y + b.x*c.y + c.x*d.y + d.x*a.y - a.y*b.x - b.y*c.x - c.y*d.x - d.y*a.x);
}

std::pair<ld, ld> dif(int i, ld l1, ld l) {
    // Finds the area given by the line going from the edge at index i at position l1 to the point
    // at distance l away along the polygon
    for(int j = i; l > 0; l -= B[j++]) { // Subtract l by the edge lengths until we get
        if(l < B[j]) {                   // to the right edge
            auto p1 = v[i] + (v[i+1]-v[i])*(l1/B[i]), p2 = v[j] + (v[j+1]-v[j])*((l)/B[j]);
            ld a2 = A[i+1][j] + area(p1, v[i+1], v[j], p2);
            return { a2, (p2-p1).d() };
        }
    }
}

ld t1(int i, ld l1) {
    // Binary search from a point on an edge to the rest of the polygon for the splitting line
    ld L = 0, R = ::L, x = 0, l = 0;
    while(R - L > 1e-6) {
        auto mid = L + (R - L)/2;
        auto p = dif(i, l1, mid);
        x = p.first, l = p.second; 
        if(x < A[0][N-1]/2)
            L = mid;
        else
            R = mid;
    }
    return l;
}

ld t2(int i) {
    // Ternary search over an edge for the shortest splitting line
    ld L = 0, R = B[i], x = 0, y = 0, l;
    while(R - L > 1e-6) {
        x = t1(i, L + (R - L)/3);
        y = t1(i, L + 2*(R - L)/3);
        if(x < y)
            R = L + 2*(R - L)/3;
        else
            L = L + (R - L)/3;
    }
    return x;
}

int main() {
    scanf("%d", &N);
    
    for(int i = 0; i < N; i++)
        scanf("%Lf %Lf", &v[i].x, &v[i].y);

    for(int i = 0; i <= N; i++) {
        v[i+N] = v[i];
        L += B[i] = B[i+N] = (v[i+1]-v[i]).d();
    }

    for(int i = 0; i < N; i++)
        for(int j = 0; j < i+N; j++) // Precalculate areas of polygon slices
            A[i][j] = A[i+N][j+N] = preArea(i, j);

    ld min = 1e300;
    for(int i = 0; i <= N; i++)
        min = std::min(min, t2(i));
    printf("%.10Lf", min);
}
