/* 1955. Boss, I Can See You! - http://acm.timus.ru/problem.aspx?num=1955
 *
 * Strategy:
 * For each pair of vertices and the line the pair forms, trace the line (starting at one of them)
 * formed by the vertices through the polygon; report the length of the longest such a line.
 *
 * Performance:
 * O(n^3), runs the tests in 0.125s using 184KB memory.
 */

#include <stdio.h>
#include <cmath>

using ld = long double;
const auto inf = std::numeric_limits<ld>::infinity();

int sgn(int f) { // Sign function
    return f > 0 ? 1 : f < 0 ? -1 : 0;
}

struct vec {
    int x, y;
    vec operator+ (const vec& p) const { return { p.x + x, p.y + y }; };
    vec operator- (const vec& p) const { return { x - p.x, y - p.y }; };
    vec operator- () const { return { -x, -y }; }
    vec operator* (int t) const { return { x*t, y*t }; };
    int operator* (const vec& p) const { return x*p.x + y*p.y; };
    int operator% (const vec& p) const { return { sgn(x*p.y - p.x*y) }; };
    vec r() { return { -y, x }; }
    ld d() { return std::sqrt(x*x + y*y); }
};

vec A[200];
int n, side;

// Intersects the line at p0 through p1 with the line segment [q0, q1]
std::pair<ld, ld> intersect(vec p0, vec p1, vec q0, vec q1) {
    auto v = q0 - q1, u = p1 - p0;
    auto b = q0 - p0;
    auto d = (u.x*v.y - v.x*u.y);
    if(!d) // Line parallel with segment, doesn't intersect
        return { inf, inf };

    auto s = (b.x*v.y - v.x*b.y)/ld(d); // Cramer's rule
    auto t = (u.x*b.y - b.x*u.y)/ld(d);
    return { s, t };
}

// Checks if the vector w extending from vertex i lies inside the polygon
bool inside(const vec& w, int i) {
    // Vectors u and v are along the sides at i
    auto u = A[(n+i-1)%n] - A[i%n], v = A[(i+1)%n] - A[i%n];
    auto uN = -u.r()*side; // The normal at u
    auto vN = v.r()*side;  // And v

    if(w*uN >= 0 && w*vN >= 0) // If both normals point in the general direction of w, we are inside
        return true;
    if(w*uN < 0 && w*vN < 0) // If they both point in the other direction, we're not
        return false;
    if(w*uN < 0) // Make u refer to the line whose normal one points along w
        std::swap(u, v), std::swap(uN, vN);

    // If the vertex with the opposing normal obscures the other one, we're outside
    return sgn(w%v) != sgn(v%u);
}

// Finds the closest intersection of the ray with origin A[i] and direction v
ld findClosest(vec v, int i) {
    ld T = inf;
    for(int j = 0; j < n; j++) {
        auto p0 = A[i], p1 = p0 + v;
        auto q0 = A[j], q1 = A[(j+1)%n];
        auto [s, t] = intersect(p0, p1, q0, q1);

        if(s == inf || s < 0)
            continue;
        if((q1-p0)%v == 0) { // Intersects q1
            if(!inside(v, j+1)) // Check that it doesn't just tangent it
                T = std::min(s*(p1-p0).d(), T);
        } else if((q0-p0)%v == 0) { // Intersects q0
            if(!inside(v, j))
                T = std::min(s*(p1-p0).d(), T);
        } else if(t > 1e-12 && t < 1) // Intersects internally
            T = std::min(s*(p1-p0).d(), T);
    }
    return T < inf ? T : 0;
}

int main() {
    scanf("%d", &n);

    for(int i = 0; i < n; i++)
        scanf("%d %d", &A[i].x, &A[i].y);

    // Use the shoelace formula to determine the orientation of the polygon
    int a = 0;
    for(int i = 0; i < n; i++)
        a += A[i].x*A[(i+1)%n].y - A[(i+1)%n].x*A[i].y;
    side = sgn(a);

    ld ans = 0;
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            // For every ray A[i] -> A[j], from the origin ..
            ld d1 = 0, d2 = 0;
            if(inside(A[j]-A[i], i))
                d1 = findClosest(A[j]-A[i], i); // .. find its closest intersection forward
            if(inside(A[i]-A[j], i))
                d2 = findClosest(A[i]-A[j], i); // and backward
            ans = std::max(ans, d1+d2);
        }
    }
    printf("%.10lf", ans);
}
