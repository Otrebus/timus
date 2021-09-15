/* 1241. Inlay Cutters - http://acm.timus.ru/problem.aspx?num=1241
 *
 * Strategy:
 * Divide the area into 2M x 2N vertices (tile intersects and tile centers). For every line, record
 * its forward and backward directions at every vertex it passes through and for every vertex and
 * every direction it contains, trace a path along the convex polygon it is uniquely part of and
 * increment the answer if the sum of the interior angles of the polygon is 180.
 *
 * Performance:
 * O(MN), runs the tests in 0.015s using 172KB memory.
 */

#include <stdio.h>
#include <algorithm>

char A[101][101]; // The directions as a bitfield; 0 is right, 1 is up+right, 2 is up, ..

// D translates direction to dx/dy
int D[8][2] = {{1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1}, {0, -1}, {1, -1}};
int M, N, K;

int sgn(int x) {
    return (x > 0) - (x < 0);
}

int to(int dx, int dy) {
    // Translates dx/dy of a line to its directions (dx = 1, dy = 0 -> 0 and 4 as in left and right)
    int x = !dy ? 0 : !dx ? 2 : dy/dx < 0 ? 3 : 1;
    return (1 << x) | (1 << (x+4));
}

bool trace(int x0, int y0, int i0) {
    // Traces a polygon ccw from the given vertex and direction, returns true if it's a triangle
    int x = x0, y = y0, i = i0, a = 0; // a = angle (in multiples of 45)

    while(true) {
        if(!(A[x][y] & (1 << i))) // Already visited
            return false;
        A[x][y] &= ~(1 << i); // Remove the direction we try from this vertex

        x += D[i][0], y += D[i][1]; // Move in the direction of the line
        if(x > M || x < 0 || y > N || y < 0)
            return false;

        if(x == x0 && y == y0) // If we reach the beginning vertex, add the proper angle and check
            return a + std::min((12+i0-i)%8, 8-(12+i0-i)%8) == 4;

        for(int a2 = 3; a2 >= 1; a2--) {
            // Check for a line turning 45, 90 and 135 degrees from us, whichever is greatest
            if(A[x][y] & (1 << ((i+a2)%8))) {
                i = (i+a2)%8;
                a += 4-a2; // The internal angle
                break;
            }
        }
    };
    return a == 4;
}

int main() {
    scanf("%d %d %d", &M, &N, &K);
    M *= 2; N *=2;

    // Add the boundary lines
    for(int i = 0; i <= M; i++)
        A[i][0] = A[i][N] |= (1 << 0) | (1 << 4);
    for(int i = 0; i <= N; i++)
        A[0][i] = A[M][i] |= (1 << 2) | (1 << 6);

    while(K--) {
        int x1, y1, x2, y2;
        scanf("%d %d %d %d", &x1, &y1, &x2, &y2);

        x1 *= 2, y1 *= 2, x2 *= 2, y2 *= 2;
        int m = to(x2-x1, y2-y1), dx = sgn(x2-x1), dy = sgn(y2-y1); // Directions, dx, dy
        // Follow the line through the board and record its two directions at every vertex
        for(int x = x1, y = y1; x >= 0 && x <= M && y >= 0 && y <= N; x += dx, y += dy)
            A[x][y] |= m;
    }

    int ans = 0;
    for(int y = 0; y <= N; y++) // At every vertex, trace a polygon 
        for(int x = 0; x <= M; x++)
            for(int i = 0; i < 3; i++) // Polygons have at least one direction between 0 and 3
                ans += trace(x, y, i); // If the polygon is a triangle, add to answer
    printf("%d", ans);
}
