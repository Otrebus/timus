/* 1386. Maze - http://acm.timus.ru/problem.aspx?num=1386
 *
 * Strategy:
 * Maintain a set of possible locations which is iteratively mapped by the given instructions.
 *
 * Performance:
 * O(MNS), runs the tests in 0.093s using 464KB memory.
 */

#include <stdio.h>
#include <cstring>

struct { int i, j; } D[5][101][101];
bool A[2][101][101];

int main() {
    int N, M, S;

    // Read the instruction mappings
    scanf("%d %d", &N, &M);
    for(int k = 1; k <= 4; k++)
        for(int j = 1; j <= N; j++)
            for(int i = 1; i <= M; i++)
                scanf("%d %d", &D[k][j][i].j, &D[k][j][i].i);

    // Map the set of possible locations into a new set by the given instruction
    std::memset(A[0], true, sizeof(A[0]));
    scanf("%d", &S);
    for(int s = 0; s < S; s++) {
        int k;
        scanf("%d", &k);
        for(int j = 1; j <= N; j++)
            for(int i = 1; i <= M; i++)
                if(A[s%2][j][i])
                    A[(s+1)%2][D[k][j][i].j][D[k][j][i].i] = true;

        // Reset the old set for reuse
        for(int j = 1; j <= N; j++)
            for(int i = 1; i <= M; i++)
                A[s%2][j][i] = false;
    }

    // Count the number of possible locations
    int L = 0;
    for(int j = 1; j <= N; j++)
        for(int i = 1; i <= M; i++)
            L += A[S%2][j][i];

    // Output them
    printf("%d\n", L);
    for(int j = 1; j <= N; j++)
        for(int i = 1; i <= M; i++)
            if(A[S%2][j][i])
                printf("%d %d\n", j, i);
}
