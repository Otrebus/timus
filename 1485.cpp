/* 1485. Football and Lie - http://acm.timus.ru/problem.aspx?num=1485
 *
 * Strategy:
 * No pair of entries (i, j) and (j, i) can both be zero, so given the truthiness of some captain
 * and its according mutation of the row of matrix values, we can create implications about the
 * truthiness of other captains (see line 83 below), and use this to prune a depth-first brute-force
 * search.
 *
 * Performance:
 * Runs the tests in 0.031s using 116KB memory.
 */

#include <stdio.h>
#include <memory>
#include <vector>

int n;
int A[100][100];    // The input
int B[100][2][100]; // B[i][t][j] = if captain i speaks the (t?)ruth, what must captain j speak?
int C[100];         // The truthiness of captains
int D[100][100];    // The output

bool dfs(int i) {
    // Finds some combination of truthiness of captains from captain i and forward, given the
    // truthiness given so far in C and the implications in B
    if(i == n)
        return true;
    for(int t = 0; t < 2; t++) { // For captain i lying or telling the truth
        bool A = true;
        for(int j = 0; j < n && A; j++) // Check that all implications match C
            if(B[i][t][j] && C[j] && B[i][t][j] != C[j])
                A = false;
        if(!A)
            continue;

        int E[100]; // Back up the current truthiness before recursing
        std::memcpy(E, C, sizeof(C));
        for(int j = 0; j < n; j++)
            C[j] = B[i][t][j] ? B[i][t][j] : C[j]; // Apply the new implications
        C[i] = t ? 1 : -1;
        if(dfs(i+1)) // Recurse
            return true;
        std::memcpy(C, E, sizeof(E)); // Restore the backup
    }
    return false;
}

std::vector<int> getT(int i, int j) {
    // Returns the possible truth values of the position (i, j) given the truthiness of captains
    // i and j
    if(A[i][j] == -1)
        return { 0, 1 };
    return A[i][j] ? std::vector<int>{ C[i] > 0 } : std::vector<int>{ C[i] < 0 };
}

void put(int i, int j) {
    // Finds some combination of scores for positions (i, j) and (j, i) that satisfies the
    // truthiness of captains i and j
    for(auto a : getT(i, j)) {
        for(auto b : getT(j, i)) {
            if(a || b) {
                if(a && b)
                    D[i][j] = D[j][i] = 1;
                if(a && !b)
                    D[i][j] = 3, D[j][i] = 0;
                if(!a && b)
                    D[i][j] = 0, D[j][i] = 3;
                return;
            }
        }
    }
}

int main() {
    scanf("%d", &n);
    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++)
            scanf("%d", &A[i][j]);

    // Deduce the necessary implications
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            if(!A[i][j] && !A[j][i]) // If both captains say they didn't score and
                B[i][1][j] = -1;     // captain i is telling the truth, j must be lying
            if(!A[i][j] && A[j][i] == 1) // Etc.
                B[i][1][j] = 1;
            if(A[i][j] == 1 && !A[j][i])
                B[i][0][j] = -1;
            if(A[i][j] == 1 && A[j][i] == 1)
                B[i][0][j] = 1;
        }
    }

    if(!dfs(0))
        return printf("Impossible"), 0;

    // Output the scores
    for(int i = 0; i < n; i++)
        for(int j = i+1; j < n; j++)
            put(i, j);
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++)
            printf("%d ", D[i][j]);
        printf("\n");
    }
}
