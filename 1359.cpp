/* 1359. Construction - http://acm.timus.ru/problem.aspx?num=1359
 *
 * Strategy:
 * Dynamic programming. Loop through each position meter by meter from top to bottom, and try
 * connecting each position with every valid previous position (up and to the right) with a ramp,
 * updating the minimum time to reach this position.
 *
 * Performance:
 * O((mn)^2), runs the tests in 0.031s using 188KB memory.
 */

#include <stdio.h>
#include <cmath>
#include <algorithm>

using ld = long double;
ld A[51][51];

void upd(int Y, int y, int X, int x)
{
    int dy = Y-y, dx = X-x, d2 = dx*dx + dy*dy;
    // Isolate forces of the ramp, integrate the acceleration to distance with the initial velocity
    // derived from the transfer of potential energy to kinetic, solve t for distance = ramp length
    ld b = 2*std::sqrt(2*10*y*d2)/(dy*10), c = -2.0l*(d2)/(dy*10);
    A[X][Y] = std::min(A[X][Y], A[x][y] - b/2 + std::sqrt(b*b/4 - c));
}

int main() {
    std::fill((ld*)A+1, (ld*)A+51*51, std::numeric_limits<ld>::infinity());
    int n, m;
    scanf("%d %d", &n, &m);
    for(int Y = 1; Y <= m; Y++)
        for(int X = 0; X <= n; X++)
            for(int y = 0; y < Y; y++)
                for(int x = 0; x <= X; x++)
                    upd(Y, y, X, x);

    printf("%.10Lf", A[n][m]);
}
