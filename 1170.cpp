/* 1170. Desert - http://acm.timus.ru/problem.aspx?num=1170
 *
 * Strategy:
 * For each vertex, intersect every rectangle with the line passing through the origin and said
 * vertex and sum the corresponding lengths and times; record and output the optimal result.
 * 
 * Performance:
 * O(N^2), runs the tests in 0.015s using 196KB memory.
 */

#include <stdio.h>
#include <array>
#include <cmath>

using ld = long double;

struct rect {
    int x1, y1, x2, y2;
    int c;
} R[500];

void isect(int x, int y, int x1, int x2, int y1, ld& xi, ld& yi) {
    // Intersects the line with endpoints x1, x2 at height y1, with line (0, 0) through (x, y)
    if(y*x1 <= x*y1 && x*y1 <= y*x2)
        xi = ld(x*y1)/y, yi = y1;
}

std::pair<ld, ld> isect(rect& r, int x, int y) {
    // Intersects rectangle r with the line passing through the origin and (x, y)
    ld xa = 0, ya = 0, xb = 0, yb = 0;
    isect(x, y, r.x1, r.x2, r.y1, xa, ya);
    isect(y, x, r.y1, r.y2, r.x1, ya, xa);
    isect(x, y, r.x1, r.x2, r.y2, xb, yb);
    isect(y, x, r.y1, r.y2, r.x2, yb, xb);
    ld l = std::sqrt((xa-xb)*(xa-xb) + (ya-yb)*(ya-yb));
    return { l, l*r.c };
}

int main() {
    int N, c, L, X, Y;
    ld T = 1e100; // Minimum time recorded so far
    scanf("%d", &N);
    for(int i = 0; i < N; i++)
        scanf("%d %d %d %d %d", &R[i].x1, &R[i].y1, &R[i].x2, &R[i].y2, &R[i].c);
    scanf("%d %d", &c, &L);

    for(int i = 0; i < N; i++) {
        for(int x : { R[i].x1, R[i].x2 }) {
            for(int y : { R[i].y1, R[i].y2 }) { // For each vertex
                ld t = 0, l = 0;
                for(int j = 0; j < N; j++) { // Intersect with each rectangle
                    auto p = isect(R[j], x, y);
                    l += p.first;
                    t += p.second;
                }
                t += (L - l)*c; // Add the desert time
                if(t < T) // New minimum time
                    X = x, Y = y, T = t; // Record parameters
            }
        }
    }
    printf("%Lf\n", T);
    printf("%Lf %Lf", L*X/std::sqrt(X*X+Y*Y), L*Y/std::sqrt(X*X+Y*Y));
}
