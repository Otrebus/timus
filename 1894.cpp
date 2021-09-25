/* 1894. Non-Flying Weather - http://acm.timus.ru/problem.aspx?num=1894
 *
 * Strategy:
 * Imagine that we copy the shape N (call it N') right next to shape M, having them share one
 * vertex, i from N' and j from M. Then we let N' move around M by incrementing the shared
 * vertex either on N' or on M as the angles of the edges permit (so the figures never intersect).
 * We then project a line from i on the original figure N to the edge (j, j+1) on M, or from j to
 * the edge (i, i+1) on N depending on if we increase j or i. Return the minimum of these line
 * lengths over all the increments as we trace N' around M.
 *
 * Performance:
 * O(N + M), runs the test cases in 0.062s using 1,748KB memory.
 */

#include <algorithm>
#include <cmath>
#include <stdio.h>

using ld = long double;

struct vec {
    ld x, y;
    vec operator-(const vec& v) { return { x-v.x, y-v.y }; }
    ld operator*(const vec& v) { return x*v.x + y*v.y; }
    ld operator%(const vec& v) { return x*v.y - v.x*y > 0; }
    ld d() { return std::sqrt(ld(x*x+y*y)); };
    ld d2() { return ld(x*x+y*y); };
};

vec N[50000], M[50000];
int n, m, sn, sm, I, J;

ld ldist(vec p, vec p1, vec p2) {
    // Returns the shortest distance from point p to line segment (p1, p2)
    auto v1 = p-p1, v2 = p2-p1;
    auto v1l = v1.d(), s = v1*v2/(v1l*v2.d()), l = (p1-p2).d();
    if(s < 0)
        return (p-p1).d();
    else if(s*v1l > l)
        return (p-p2).d();
    return std::sqrt(v1.d2() - s*s*v1.d2());
}

int main() {
    scanf("%d %d", &n, &m);

    for(int i = 0; i < n; i++)
        scanf("%Lf %Lf", &N[i].x, &N[i].y);

    for(int i = 0; i < m; i++)
        scanf("%Lf %Lf", &M[i].x, &M[i].y);

    // We put N' to the right of M, so find their min/max vertices along the x axis
    sn = std::min_element(N, N+n, [] (auto& v, auto& w) { return v.x < w.x; })-N;
    sm = std::min_element(M, M+m, [] (auto& v, auto& w) { return v.x > w.x; })-M;

    ld min = 1e100;
    while(I < n || J < m) {
        int i = (I+sn)%n, j = (J+sm)%m, i1 = (i+1)%n, j1 = (j+1)%m;

        if(J == m || (N[i]-N[i1])%(M[j1]-M[j]) > 0) {
            // We move N' along the edge (i, i+1); check the distance from j to (i, i+1)
            min = std::min(min, ldist(M[j], N[i], N[i1]));
            I++;
        } else {
            // We move M along the edge (j, j+1); check the distance from i to (j, j+1)
            min = std::min(min, ldist(N[i], M[j], M[j1]));
            J++;
        }
    }
    printf("%.10Lf", std::max(min-60, 0.l));
}
