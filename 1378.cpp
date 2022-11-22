/* 1378. Artificial Intelligence - http://acm.timus.ru/problem.aspx?num=1378
 *
 * Strategy:
 * Calculate the smallest distance d from the centroid to the boundary. The area of a circle should
 * be roughly pi*d^2, a square 4*d^2, and greater for a triangle.
 *
 * Performance:
 * O(W*H), runs the tests in 0.031s using 1,444KB memory.
 */

#include <stdio.h>
#include <array>
#include <algorithm>

struct point {
    point(double x, double y) : x(x), y(y) {}
    point operator +=(point p) { return { x += p.x, y += p.y }; };
    point operator /=(double t) { return { x /= t, y /= t }; };
    point operator -(point p) { return { x - p.x, y - p.y }; };
    double d2() {
        return x*x + y*y;
    }
    double x, y;
};

char A[1001][1001];

int main() {
    int W, H;
    scanf("%d %d", &W, &H);
    for(int x = 0; x < H; x++)
        scanf("%s", A[x]);

    double r2 = 1e300, a = 0; // Minimum squared distance from centroid to edge, area
    point C(0, 0); // Centroid

    // Calculate the centroid
    for(int x = 1; x < H-1; x++)
        for(int y = 1; y < W-1; y++)
            if(A[x][y] == '1')
                C += point(x, y), a += 1;
    C /= a;

    // Get the minimum squared distance from the centroid to the edge
    for(int x = 1; x < H-1; x++)
        for(int y = 1; y < W-1; y++)
            for(auto c : { A[x+1][y], A[x-1][y], A[x][y+1], A[x][y-1] })
                if(c != A[x][y])
                    r2 = std::min((C-point(x, y)).d2(), r2);

    printf(a < r2*4 ? "circle" : a < r2*5 ? "square" : "triangle");
}
