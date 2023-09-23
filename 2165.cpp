/* 2165. Cake Cutter - http://acm.timus.ru/problem.aspx?num=2165
 *
 * Strategy:
 * Dynamic programming over A[i][n], where i is a starting vertex and n a number of vertices, which
 * denotes the solution of the problem for the polygon with vertices [i, i+n). To triangulate some
 * given [i, i+n)-polygon we can either:
 *
 *   a) Choose not to draw any diagonals from i in which case we calculate the area for the triangle
 *      {i, i+1, i+n-1}  and the subsolution for the rest of the polygon [i+1, i+n-1) and take the
 *      minimum of those two.
 *
 *   b) For each sensible t we calculate the area of triangle {i, i+t, i+n-1} and the solutions
 *      for polygons [i, i+t] and [i+t, i+n) and take the minimum of the area and solutions.
 *
 * The solution for A[i][n] is then the maximum over the the diagonal-less case in a) and all the
 * values acquired in b).
 *
 * Performance:
 * O(N^2), runs the tests in 0.031s using 488KB memory.
 */

#include <stdio.h>
#include <cmath>
#include <limits>
#include <algorithm>

using ld = long double;
const ld inf = std::numeric_limits<ld>::infinity();
const int maxn = 200;

// Since we don't have multiparameter min yet apparently
template<typename T> T min(T a) { return a; }
template<typename T, typename... Ts> T min(T a, Ts... args) { return std::min(a, min(args...)); }

struct point {
    ld x, y;
    point operator-(const point& p) { return { x-p.x, y-p.y }; };
    ld operator%(const point& p) { return std::abs(x*p.y - p.x*y); }
} P[maxn];

int N;
ld A[maxn][maxn+1]; // DP matrix

ld area(int i, int j, int k) { // Calculates the area of the triangle with the given indices
    return (P[j%N]-P[i])%(P[k%N]-P[i])/2;
}

ld solve(int a, int n) {
    if(A[a][n] > 0)
        return A[a][n];
    ld max = std::min(area(a, a+1, a+n-1), solve(a+1, n-1)); // Case a
    for(int i = 2; i < n-1; i++) // Case b
        max = std::max(max, min(area(a, a+i, a+n-1), solve(a, i+1), solve(a+i, n-i)));
    return A[a][n] = max;
}

int main() {
    scanf("%d", &N);
    for(int i = 0; i < N; i++)
        scanf("%Lf %Lf", &P[i].x, &P[i].y);
    for(int i = 0; i < N; i++)
        for(int j = 0; j <= N; j++) // Initialize the DP matrix
            A[i][j] = j < 3 ? inf : j == 3 ? area(i, i+1, i+2) : -inf;
    printf("%.15Lf", solve(0, N));
}
