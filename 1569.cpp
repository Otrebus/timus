/* 1569. Networking the "Iset" - http://acm.timus.ru/problem.aspx?num=1569
 *
 * Strategy:
 * First, precalculate the all-source shortest paths and the associated predecessor matrix. We then
 * use that data to quickly calculate, for each possible center of the tree (which can be either
 * an edge or a vertex), the diameter associated with that center, and pick the center with the
 * smallest diameter. We can then reconstruct its associated tree (and answer) using the predecessor
 * graph.
 *
 * Performance:
 * O(N^3), runs the tests in 0.015s using 424KB memory.
 */

#include <iostream>

int P[101][101]; // Predecessor
int C[101][101]; // Distance (cost)
int A[101][101]; // Adjacency matrix
int O[101][101]; // Output

int N, M, i, j;

int updateMax(int x, int& m, int& M) {
    // Updates M (largest value) or m (second largest) given x
    if(x <= M)
        return m = std::max(x, m);
    return M = std::max(x, M);
}

int main() {
    
    std::cin >> N >> M;

    // Initialize the cost matrix and read the adjacency data
    std::memset(C, 0x3f, sizeof(C));
    for(int i = 1; i <= N; i++)
        C[i][i] = 0;

    for(int m = 0; m < M; m++) {
        std::cin >> i >> j;
        C[i][j] = C[j][i] = A[i][j] = 1;
        P[i][j] = i;
        P[j][i] = j;
    }

    for(int k = 1; k <= N; k++) // Floyd-Warshall
        for(int i = 1; i <= N; i++)
            for(int j = 1; j <= N; j++)
                if(C[i][k] + C[k][j] < C[i][j])
                    C[i][j] = C[i][k] + C[k][j], P[i][j] = P[k][j];

    int minc = 101, mini, minj;
    for(int i = 1; i <= N; i++) { // Try every center edge (or point if i = j)
        for(int j = 1; j <= N; j++) {

            if(i != j && !A[i][j]) // If an edge isn't connected it can't be a center
                continue;

            int di = 0, dj = 0, di2 = 0, dj2 = 0; // Largest/second largest path distances

            // For each node, use it to update the longest path through this center candidate
            for(int k = 1; k <= N; k++) {
                if(i == j) {
                    updateMax(C[i][k], di, dj); // Update the two longest paths from this point
                } else { // Try an edge for the center
                    if(C[i][k] < C[j][k]) // For both endpoints, update their longest paths
                        updateMax(C[i][k], di, di2);
                    else
                        updateMax(C[j][k], dj, dj2);
                }
            }
            int c = 0;
            if(i == j) // Diameter through this vertex center is the sum of the two longest paths
                c = di + dj;
            else // The diameter either goes through the edge or through one of the endpoints
                c = std::max(di2 + dj2 + 1, std::max(di + di2, dj + dj2));

            if(c < minc) // Update the smallest center
                mini = i, minj = j, minc = c;
        }
    }

    for(int j = 1; j <= N; j++) {   // Construct the spanning tree using the predecessor matrix
        int i = minj;               // by, for each vertex, tracing a shortest path back to its
        if(C[mini][j] < C[minj][j]) // closest center vertex (of the two we arrived at)
            i = mini;

        for(int k = j; P[i][k]; k = P[i][k])
            O[P[i][k]][k] = 1;
    }
    if(mini != minj)
        O[mini][minj] = 1;

    for(int i = 1; i <= N; i++)
        for(int j = 1; j <= N; j++)
            if((O[i][j] || O[j][i]) && A[i][j])
                std::cout << i << " " << j << std::endl;
}
