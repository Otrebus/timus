/* 1623. Fractal Labyrinth - https://acm.timus.ru/problem.aspx?num=1623
 *
 * Strategy:
 * Repeated Floyd-Warshall. In each iteration we treat the inner houses as black boxes that connect
 * to the outer house and themselves, and calculate the all-pairs shortest paths. Then, to each of
 * the inner houses we assign the path costs of the outer layer to itself, assign the input
 * connections, and repeat.
 *
 * Performance:
 * O(K^3*N^4), runs the tests in 0.046s using 216KB memory.
 */

#include <stdio.h>
#include <cstring>
#include <algorithm>

int A[6][20][6][20];  // The path costs between the outer house to the inner houses, and themselves
bool T[6][20][6][20]; // The connections as given in the input
const int inf = 0x1f1f1f1f;

int main() {
    int N, K, M, Di, Do, a, b, c, d;
    scanf("%d %d %d", &N, &K, &M);
    std::memset(A, 0x1f, sizeof(A));

    // Read the connections
    while(M--) {
        scanf("%d.%d - %d.%d", &a, &b, &c, &d);
        T[a][b][c][d] = T[c][d][a][b] = true;
    }

    // Self-connections are free
    for(int x = 0; x <= K; x++)
        for(int i = 0; i < N; i++)
            A[x][i][x][i] = 0;
    
    for(int a = 0; a <= N; a++) {

        // Assign the input connections
        for(int i = 0; i < N; i++)
            for(int j = 0; j < N; j++)
                for(int x = 0; x <= K; x++)
                    for(int y = 0; y <= K; y++)
                        if(T[x][i][y][j])
                            A[x][i][y][j] = T[x][i][y][j];

        // Calculate the all-pairs shortest paths
        for (int k = 0; k < N; k++)
            for(int z = 0; z <= K; z++)
                for(int i = 0; i < N; i++)
                    for(int x = 0; x <= K; x++)
                        for(int j = 0; j < N; j++)
                            for(int y = 0; y <= K; y++)
                                A[x][i][y][j] = std::min(A[x][i][z][k]+A[z][k][y][j],A[x][i][y][j]);
        
        // Copy the intra-cost of the outer layer to each of the inner houses
        for(int x = 1; x <= K; x++)
            for(int i = 0; i < N; i++)
                for(int j = 0; j < N; j++)
                    A[x][i][x][j] = A[0][i][0][j];
    }

    scanf("%d %d", &Di, &Do);
    if(A[0][Di][0][Do] >= inf)
        printf("no solution");
    else
        printf("%d", A[0][Di][0][Do]);
}
