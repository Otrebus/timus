/* 2139. Experiment with Juice - http://acm.timus.ru/problem.aspx?num=2139
 *
 * Strategy:
 * Precalculate a prefix array of the area of the polygon from each edge to the centroid of the
 * polygon and use that array to calculate the area of the polygon below the horizontal line
 * of the hole after each query. Also keep track of when the line segment that the hole is on
 * becomes horizontal and lowest.
 *
 * Performance:
 * O(qlog n), runs the tests in 0.359s using 2,280KB memory.
 */

#include <stdio.h>
#include <cmath>
#include <iostream>
#include <vector>

using ld = long double;
const ld pi = std::acos(-1);

struct vec {
    ld x, y;
    vec operator+ (const vec& p) const { return { p.x + x, p.y + y }; };
    vec operator- (const vec& p) const { return { x - p.x, y - p.y }; };
    vec operator* (ld t) const { return { x*t, y*t }; };
    vec operator/ (ld t) const { return { x/t, y/t }; };
    ld operator* (const vec& p) const {
        return x*p.x + y*p.y;
    }
    ld operator% (const vec& p) const { return x*p.y - y*p.x; }
    ld d() const { return std::sqrt(x*x + y*y); }
    vec n() const { return vec{ x, y }/d(); }
};

int n;
ld a, h;
vec c;
ld cosa = 1, sina = 0;

vec rotC(const vec& v) { // Rotates a vector by a predefined value
    return { v.x*cosa - v.y*sina, v.x*sina + v.y*cosa };
}

vec rot(ld a, const vec& v) { // Rotates a vector
    return { v.x*std::cos(a) - v.y*std::sin(a), v.x*std::sin(a) + v.y*std::cos(a) };
}

struct vecs {
    vec B[100000];
    const vec get(int i) const { return rotC(a, B[(n+i)%n]); }
    vec& set(int i) { return B[(n+i)%n]; }
} A;

ld C[100001];

ld getArea(int i, int j) { // Returns the area of the polygon pie between vertices i and j
    if(j >= i)
        return (C[j]-C[i]);
    return C[n] - C[i]+C[j];
}

int sgn(ld x) { // Signum function
    return (0 < x) - (x < 0);
}

ld ang(vec u, vec v, int dir) { // Returns the signed angle between two vectors
    auto a = std::acos(u.n()*v.n());
    if(dir == sgn(u%v))
        return a;
    return -a;
}

int dir(int i, int d) { // The direction of the given edge
    return sgn(A.get(i+d).y-A.get(i).y);
}

int findExtreme(int b) { // Finds the highest or lowest point on the polygon
    int l = 0, r = n-1;
    while(true) {
        auto m = (1+r+l)/2;
        if(dir(l, 1)*b < 0) {
            if(dir(m, -1)*b <= 0 || A.get(m).y*b > A.get(l).y*b) {
                r = m, l++;
            } else {
                l = m;
            }
        } else {
            if(dir(l, -1)*b >= 0)
                return l;
            if(dir(m, 1)*b >= 0 && A.get(m).y*b < A.get(l).y*b) {
                r = m;
            } else {
                l = m;
            }
        }
    }
}

// Finds the indices of the polygon between which where we find a vertical coordinate
// on its left or right side
int findCut(ld h, int l, int r, int b) {
    while(true) {
        auto m = (l+(r-l+n)%n/2)%n;
        if(h*b > A.get(m+1).y*b)
            l = m+1;
        else if(h*b < A.get(m).y*b)
            r = m;
        else
            return m;
    }
}

vec cutY(ld h, vec u, vec v) { // Finds the point on a line segment at a y coordinate
    if(h == u.y && h == v.y)
        return u;
    auto t = (h-u.y)/(v.y-u.y);
    return u + (v-u)*t;
}

ld triArea(vec u, vec v, vec m) { // Returns the area of a triangle
    return std::abs((v-u)%(m-u))/2;
}

// Returns the area of the polygon below y = h
ld calcArea(ld h) {
    // Find the top and bottom vertices
    int iM = findExtreme(-1);
    int im = findExtreme(1);
    auto i = findCut(h, im, iM, 1);
    auto j = findCut(h, iM, im, -1);

    ld Ap = getArea(j+1, i);

    // The points where the edge of the triangle intersect h
    auto l = cutY(h, A.get(j), A.get(j+1));
    auto r = cutY(h, A.get(i), A.get(i+1));
    if(c.y < h) { // The centroid is below h

        // Intersects of the vertices just above y=h of the polygon to the center
        auto u = cutY(h, A.get(i+1), c);
        auto v = cutY(h, A.get(j), c);

        // The area of that given triangle
        auto midArea = triArea(u, v, c);

        // The areas of the quads between u and v to the edges and the center
        auto leftArea = triArea(c, v, l) + triArea(c, l, A.get(j+1));
        auto rightArea = triArea(c, u, r) + triArea(c, r, A.get(i));

        auto ret = Ap + midArea + leftArea + rightArea;
        return ret;
    } else {
        // Sort of similarly
        auto u = cutY(h, A.get(i), c);
        auto v = cutY(h, A.get(j+1), c);
        auto midArea = triArea(u, v, c);
        auto leftArea = triArea(l, v, A.get(j+1));
        auto rightArea = triArea(r, u, A.get(i));
        auto ret = Ap - midArea + leftArea + rightArea;
        return ret;
    }
}

int main() {
    scanf("%d", &n);

    for(int i = 0; i < n; i++) {
        ld x, y;
        scanf("%lf %lf", &x, &y);
        A.set(i) = vec { x, y };
    }

    // Calculate the centroid
    for(int i = 0; i < n; i++)
        c = c + A.get(i)/n;

    vec o;
    scanf("%lf %lf", &o.x, &o.y);

    // Find the index of the vertices just after which the hole is at
    int ci = 1;
    ld md = 1;
    for(int i = 0; i < n; i++) {
        auto d = (A.get(i)-o).n()*(A.get(i+1)-o).n();
        if(d < md)
            md = d, ci = i;
    }

    // The angle of the line segment that the hole is at
    ld phi = ang(vec{-1, 0}, o-A.get(ci), 1);

    // Calculate the area prefix array
    ld area = 0;
    for(int i = 1; i <= n; i++)
        C[i] = (area += (A.get(i-1)-c)%(A.get(i)-c)/2);

    int q;
    ld da = 0;
    scanf("%d", &q);
    for(int i = 0; i <= q; i++) {
        if(i)
            scanf("%lf", &da);

        // Update all angles
        da = pi*da/180;
        a += da, phi += da;

        cosa = std::cos(a), sina = std::sin(a);

        if(phi >= pi || phi <= -pi) { // Hole segment is horizontal and low
            area = 0;
        } else if(area > 0) { // Recalculate area
            c = rot(da, c); // New centroid pos
            area = std::min(area, calcArea(rot(a, o).y));
        }
        printf("%.10lf\n", area);
    }
}
