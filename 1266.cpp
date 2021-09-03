/* 1266. Kirchhoff's Law - http://acm.timus.ru/problem.aspx?num=1266
 *
 * Strategy:
 * Set the voltage of the first node to 1, set up the equations for Kirchoff's current law
 * ((u-u1)/R + (u-u2)/R + ... = 0) and solve the resulting system using Gaussian elimination.
 * Since we then know the voltages of each node we can calculate the current leaving the first node
 * and hence the resistance of the circuit.
 *
 * Performance:
 * O(N^3), runs the test cases in 0.015s using 312KB memory.
 */

#include <iostream>

using ld = long double;
ld R[22][22]; // The resistances between the nodes
ld A[22][22]; // The coefficient matrix

void addTo(int j, int i, ld d, int N) { // Adds a multiple of one row of the matrix to another
    for(int k = 1; k <= N+1; k++)
        A[j][k] += d*A[i][k];
}

int main() {
    std::fill((ld*) R, (ld*) R + 22*22, std::numeric_limits<ld>::infinity());
    int N, M, a, b;
    ld r;
    std::cin >> N >> M;
    while(M--) {
        std::cin >> a >> b >> r;
        R[a][b] = R[b][a] = 1/(1/R[a][b] + 1/r); // Parallelize the resistance with any existing one
    }
    A[1][1] = A[N][N] = 1; // The eqs for u_1 and u_n are u_1 = 1 and u_n = 0
    for(int i = 2; i < N; i++)
        for(int j = 1; j <= N; j++)
            A[i][i] -= 1/R[i][j], A[i][j] += 1/R[i][j]; // The coefficients of the current law
 
    A[1][N+1] = 1, A[N][N+1] = 0; // The known voltages
    for(int i = 1; i <= N; i++) { // Gaussian elimination
        ld p = A[i][i];
        for(int j = 1; j <= N+1; j++) // Normalize row by pivot element
            A[i][j] /= p;
        for(int j = 1; j <= N; j++) // Eliminate the pivot column from the other rows
            if(i != j)
                addTo(j, i, -A[j][i], N);
    }

    ld s = 0;
    for(int i = 2; i <= N; i++)       // Knowing the voltages of all neighboring nodes of u_1 we
        s += (1 - A[i][N+1])/R[1][i]; // can derive the current going through u_1 and hence the
    std::cout << (1.0/s);             // resistance of the entire circuit since its voltage is 1
}
