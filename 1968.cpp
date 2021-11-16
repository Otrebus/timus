/* 1968. Glass Pyramid - http://acm.timus.ru/problem.aspx?num=1968
 *
 * Strategy:
 * We are essentially dealing with two layers of block columns, below called the A and B layers.
 * As we read the input, we can always use the input for the position and the answer for one
 * column in that position to deduce the answer for the other column (of the other layer) in that
 * position.
 *
 * Performance:
 * O(mn), runs the tests in 0.015s using 152KB memory.
 */

#include <stdio.h>
#include <cmath>
#include <algorithm>

bool V[20][20]; // If the column of layer A at this position was visited already
int A[20][20], B[20][20]; // Each layer
int m, n, c; // Input

int main() {
    scanf("%d %d", &m, &n);

    for(int y = 0; y < m; y++) {
        for(int x = 0; x < n; x++) {

            // Derive the position and highest level of each layer
            int xa = x/2, ya = y/2; // Position of A layer
            int xb = (x-1)/2, yb = (y-1)/2; // Position of B layer
            int yd = std::min(ya, m/2-ya); // Y distance for layer A to side
            int xd = std::min(xa, n/2-xa); // X distance for layer A to side
            int ha = 1 + std::min(xd, yd); // Height of layer A
            int ye = std::min(yd, m/2-yd); // Y distance for layer B to side
            int xe = std::min(xd, n/2-xd); // X distance for layer B to side
            int hb = std::min(xe, ye); // Height of layer B

            scanf("%d", &c);
            // Compare how many red blocks this square should have compared to how many we assigned
            // so far, and if there's a difference, we assign it to the corresponding A layer if
            // unvisited, otherwise the B layer
            int dif = c - (A[xa][ya] + std::min(hb, B[xb][yb])); // Layer B might have height 0
            if(dif) {
                if(!V[xa][ya])
                    A[xa][ya] += std::min(ha - A[xa][ya], dif); // Add what we can to layer A
                else
                    B[xb][yb] += c - A[xa][ya]; // Layer B must contain this
            }
            V[xa][ya] = true;
        }
    }

    // Output the answer - basically keep iterating over the grid with ever-shrinking boundaries
    int M = m, N = n;
    for(int u = 0, l = 0; u <= M && l <= N; u++, M--, l++, N--, printf("\n")) {
        int h = 1 + std::min(u, l);
        for(int y = u; y < M; y += 2, printf("\n"))
            for(int x = l; x < N; x += 2)
                // Assign any red blocks to the lowest levels possible
                printf((u % 2) ? B[x/2][y/2] >= h/2 ? "R" : "W" : A[x/2][y/2] > h/2 ? "R" : "W");
    }
}
