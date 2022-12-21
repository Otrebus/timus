/* 1733. Forgotten Technology - http://acm.timus.ru/problem.aspx?num=1733
 *
 * Strategy:
 * Do a depth-first search to a shallow depth for a path between the following points and
 * report the shortest path found:
 * 
 *   * The mouse's starting point
 *   * The vertices of the rectangle
 *   * The closest point of the circle to the current point
 *   * The intersections between the circle and the rectangle
 * 
 * Performance:
 * Constant, runs the tests in 0.015s using an unreported amount of memory.
 */

#include <cmath>
#include <vector>
#include <stdio.h>

using ll = long long;
using ld = long double;

template <typename T> struct point {
    T x, y;
    point (T x, T y) : x(x), y(y) {}
    point (const point<ll>& p) : x(p.x), y(p.y) {}
    point () {}
    point operator+ (const point& a) const { return { x+a.x, y+a.y }; }
    point operator- (const point& a) const { return { x-a.x, y-a.y }; }
    T operator* (const point& a) const { return x*a.x + y*a.y; }
    T operator% (const point& a) const { return x*a.y - y*a.x; }
    point operator/ (T f) const { return { x/f, y/f }; }
    point operator* (T f) const { return { x*f, y*f }; }
    bool operator == (const point& b) { return x == b.x && y == b.y; }
    bool operator != (const point& b) { return x != b.x || y != b.y; }
    point n() { return (*this)/std::sqrt(x*x+y*y); }
    ld d() { return std::sqrt(x*x+y*y); }
    point r() { return { -y, x }; }
};

point<ll> A[4];
point<ll> c;
ll R;
point<ll> m;
const ld inf = 1e300, eps = 1e-6;

std::vector<std::pair<bool, point<ld>>> intersect(point<ll> u, point<ll> v, point<ll> c, ll R) {
    // Line segment/circle intersection, returns the intersection points along with info
    // whether the intersection is a tangent of the interior of the line

    // We solve the equation |u + (v-u)*t|^2 = R^2, a second degree equation
    auto A = (v-u)*(v-u), B = (u-c)*(v-u)*2, C = ((u-c)*(u-c) -R*R);
    auto D = B*B/ld(4*A*A) - (C/ld(A));

    auto a = -B/ld(2*A), d = std::sqrt(ld(std::max(0.0l, D)));
    if(B*B - 4*A*C == 0) { // A single solution, tangent
        auto t = -B/ld(2*A) - std::sqrt(std::max(0.0l, D));
        point<ld> vl = v, ul = u;
        return { { std::abs(t) > eps && std::abs(t) < 1-eps, ul + (vl-ul)*t } };
    }

    if(B*B - 4*A*C > 0) { // Multiple solutions for the line, maybe less for a segment
        point<ld> vl = v, ul = u;
        std::vector<std::pair<bool, point<ld>>> ret;
        
        auto t1 = a + d, t2 = a - d;
        if(t1 > -eps && t1 < 1+eps)
            ret.push_back({ false, ul + (vl-ul)*t1 });
        if(t2 > -eps && t2 < 1+eps)
            ret.push_back({ false, ul + (vl-ul)*t2} );
        return ret;
    }
    return {};
}

std::pair<bool, point<ld>> intersect(point<ld> p0, point<ld> p1, point<ld> q0, point<ld> q1) {
    // Intersects two line segments
    auto v = q0 - q1, u = p1 - p0;
    auto b = q0 - p0;
    auto d = (u.x*v.y - v.x*u.y);
    if(!d)
        return { false, {} };

    auto s = (b.x*v.y - v.x*b.y)/ld(d); // Cramer's rule

    if(s > 1+eps || s < -eps)
        return { false, {} };

    return { true, { p0 + (p1-p0)*s } };
}

bool inside(point<ld> p) {
    // Checks if p is inside the rectangle
    bool b = true;
    for(int i = 0; i < 4; i++)
        b = b && ((p-A[i])%(p-A[(i+1)%4]) > 0);
    return b;
}

ld dfs(point<ll> p, int depth) {
    // Solves the problem by connecting a path between the points described in the header comment
    // and selecting the shortest path within a few steps
    if(depth > 5)
        return inf;

    point<ld> pl = p, cl = c;

    ld mc = inf; // Min distance (to the circle initially)
    // Try connecting to the circle directly, using some helpers:

    // Calculate the points of the circle that are only touching the rectangle
    std::vector<point<ld>> T;
    for(int i = 0; i < 4; i++) {
        auto p1 = A[i], p2 = A[(i+1)%4];
        auto vs = intersect(p1, p2, c, R);
        if(vs.size() == 1 && vs[0].first)
            T.push_back(vs[0].second);
    }

    // Checks if the given point is only touching the rectangle
    const auto isTouching = [&T] (const point<ld>& p) {
        for(auto t : T)
            if((p-t).d() < eps)
                return true;
        return false;
    };

    // Connect to the points of intersection of the circle with the line segment ..
    for(int i = 0; i < 4; i++) {
        auto p1 = A[i], p2 = A[(i+1)%4];
        auto vs = intersect(p1, p2, c, R);

        for(auto v : vs) {
            if(isTouching(v.second))
                continue;
            
            // We don't want to intersect other segments
            bool bad = false;
            if(p != A[i] && p != A[(i+1)%4]) {
                for(int j = 0; j < 4; j++) {
                    if(j != i) {
                        point<ld> vl = v.second;
                        auto [b, pi] = intersect(A[j], A[(j+1)%4], p, vl);
                        if(b && (pi-pl).d() < (pl-vl).d()) {
                            bad = true;
                        }
                    }
                }
            }
            if(!bad)
                mc = std::min(mc, (v.second-pl).d());
        }
    }

    // Connect directly to the circle
    auto pc = cl + (pl-cl).n()*R;
    if(!inside(pc) && !isTouching(pc)) // Can't connect to something inside the rectangle
        mc = std::min(mc, (pc - pl).d());

    if(mc != inf)
        return mc;

    if(p != m) { // We are a vertex, find who we are and connect to the neighboring two
        for(int i = 0; i < 4; i++) {
            if(p == A[i]) {
                auto a = (p-A[(i+1)%4]).d() + dfs(A[(i+1)%4], depth+1);
                auto b = (p-A[(i+3)%4]).d() + dfs(A[(i+3)%4], depth+1);
                return std::min(a, b);
            }
        }
    } else { // Connect between the starting point to a vertex
        for(int i = 0; i < 4; i++) {
            // Can only connect to vertices whose line face us correctly
            if((A[i]-p) * (A[(i+1)%4]-A[i]).r() >= 0) {
                auto a = (p-A[i]).d() + dfs(A[i], depth+1);
                auto b = (p-A[(i+1)%4]).d() + dfs(A[(i+1)%4], depth+1);
                mc = std::min(mc, std::min(a, b));
            }
        }
    }
    return mc;
}

int main() {
    for(int i = 0; i < 4; i++)
        scanf("%lld %lld", &A[i].x, &A[i].y);
    scanf("%lld %lld %lld %lld %lld", &c.x, &c.y, &R, &m.x, &m.y);
    ld ans = dfs(m, 0);
    printf("%.10Lf", ans);
}

