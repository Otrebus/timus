/* 1766. Humpty Dumpty - http://acm.timus.ru/problem.aspx?num=1766
 *
 * Strategy:
 * The process is a Markov chain, so we set up the stochastic matrix and solve for the eigenvector
 * for eigenvalue 1, which is the steady-state probability vector we're looking for.
 *
 * Performance:
 * O(N^3), where N is the number of rows, runs the tests in 0.015s using 648KB memory
 */

#include <iostream>
#include <iomanip>

using ld = long double;

ld A[64][65]; // Stochastic matrix
int H[8][8];  // Input

void addTo(int j, int i, ld d) { // Adds a multiple of one row of the matrix to another
    for(int k = 0; k < 65; k++)
        A[j][k] += d*A[i][k];
}

int main() {
    for(int y = 0; y < 8; y++)
        for(int x = 0; x < 8; x++)
            std::cin >> H[x][y];

    for(int y = 0; y < 8; y++) {
        for(int x = 0; x < 8; x++) {
            ld s = 0;
            // The probability to jump to a square is proportional to the number in it which
            // must mean relative to the sum of all neighbors
            for(int dx = -1; dx <= 1; dx++)
                for(int dy = -1; dy <= 1; dy++)
                    if(dx || dy)
                        if(x+dx >= 0 && x+dx < 8 && y + dy >= 0 && y+dy < 8)
                            s += H[x+dx][y+dy];

            // Set up the transition probabilities in the stochastic matrix
            for(int dx = -1; dx <= 1; dx++)
                for(int dy = -1; dy <= 1; dy++)
                    if(dx || dy)
                        if(x+dx >= 0 && x+dx < 8 && y + dy >= 0 && y+dy < 8)
                            A[x+dx+(y+dy)*8][x+y*8] = H[x+dx][y+dy]/s;
        }
    }

    for(int i = 0; i < 64; i++) // The I part of (A-I)x = 0
        A[i][i] -= 1;

    for(int j = 0; j < 65; j++)
        A[63][j] = 1; // We want the vector that sums to 1 (also, system is homogeneous otherwise)

    // Gaussian elimination
    for(int i = 0; i < 64; i++) { // For each diagonal ..
        ld p = A[i][i];
        // Normalize the row by the pivot position
        for(int j = 0; j < 65; j++)
            A[i][j] /= p;
        // Subtract all above rows by this row
        for(int j = 0; j < 64; j++)
            if(i != j)
                addTo(j, i, -A[j][i]);
    }

    std::cout << std::fixed << std::setprecision(15);
    for(int y = 0; y < 8; y++) {
        for(int x = 0; x < 8; x++)
            std::cout << A[x+y*8][64] << " ";
        std::cout << std::endl;
    }
}
