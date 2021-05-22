/* 1317. Hail - http://acm.timus.ru/problem.aspx?num=1317
 *
 * Strategy:
 * Straightforward line/segment intersection.
 *
 * Performance:
 * O(nk), runs the tests in 0.001s using 264KB memory.
 */

#include <stdio.h>
#include <vector>
#include <cmath>
#include <numeric>

using ld = long double;
const ld eps = 1e-9;

struct point {
    ld x, y;
    point operator-(const point& a) const { return { x - a.x, y - a.y }; }
    bool operator==(const point& a) const { return x == a.x && y == a.y; }
    ld len() const { return std::sqrt(x*x + y*y); }
};

point f[10]; // Fence posts
point v[100]; // Hail stones
bool ans[100]; // Marks stones reachable

void intersect(ld& s, ld& t, point p1, point p2, point q1, point q2) {
    // Solves p1 + s*p2 = q1 = t*q2 using Cramer's rule
    point d = q1 - p1;
    ld det = q2.x*p2.y - p2.x*q2.y;
    s = (q2.x*d.y - d.x*q2.y)/det;
    t = (p2.x*d.y - d.x*p2.y)/det;
}

int main() {
    int n, k;
    ld h, d, lx, ly, s, t;

    // Read all input
    scanf("%d %Lf", &n, &h);
    for(int i = 0; i < n; i++)
        scanf("%Lf %Lf", &f[i].x, &f[i].y);
    scanf("%Lf %Lf %Lf %d", &d, &lx, &ly, &k);
    for(int i = 0; i < k; i++)
        scanf("%Lf %Lf", &v[i].x, &v[i].y);

    for(int i = 0; i < n; i++) { // For each fence segment
        point p1 = f[i], p2 = f[(i+1)%n] - f[i];
        for(int j = 0; j < k; j++) { // For each hailstone
            point q1 = { lx, ly }, q2 = (v[j] - q1);
            intersect(s, t, p1, p2, q1, q2);
            // If the hailstone is right on top of us, or the laser intersects some fence
            if(q1 == v[j] || s >= -eps && s <= 1+eps && t >= 0)
                // Determine the minimum height by similar triangles, check if within range
                if(std::sqrt(h*h/t/t + q2.len()*q2.len()) < d + eps)
                    ans[j] = true;
        }
    }

    printf("%d", std::accumulate(ans, ans+100, 0));
}
