/* 1768. Circular Strings - http://acm.timus.ru/problem.aspx?num=1768
 *
 * Strategy:
 * Translate the vectors by their centroid, and check that they rotate around the centroid by angle
 * 2pi/N, all at the same length from the centroid.
 *
 * Performance:
 * O(n), runs the tests in 0.031s using 284KB memory.
 */

#include <iostream>
#include <algorithm>

using ld = long double;
ld pi = 3.14159265358979323846264338327950288419716939937510L;

struct vec {
    ld x, y;
    vec operator-(const vec& pt) const { return { x-pt.x, y-pt.y }; }
    vec operator+(const vec& pt) const { return { pt.x+x, pt.y+y }; }
    ld len() const { return std::sqrt(x*x+y*y); };
} p[100];
int n;

bool check(int d) { // Checks that the points rotate with direction d (ccw vs cw)
    ld a = 2*pi/n;
    ld a1 = std::atan2(p[0].y, p[0].x), l = p[0].len();
    for(int i = 1; i < n; i++) {
        // Each point is expected to increase its angle by 2*pi/n and have length l
        vec v = { std::cos(a1+d*a*i)*l, std::sin(a1+d*a*i)*l };
        if((v-p[i]).len() > 1e-5)
            return false;
    }
    return true;
}

int main() {
    std::cin >> n;
    vec m { 0, 0 };
    for(int i = 0; i < n; i++) {
        std::cin >> p[i].x >> p[i].y;
        m = m + p[i];
    }
    m.x /= n, m.y /= n; // Calculate the centroid
    for(int i = 0; i < n; i++)
        p[i] = p[i] - m; // Translate by the centroid

    printf(!check(1) && !check(-1) ? "NO" : "YES");
}
