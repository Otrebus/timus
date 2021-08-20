/* 1508. Japanese Puzzle - http://acm.timus.ru/problem.aspx?num=1508
 *
 * Strategy:
 * Dynamic programming. For every cell c, pick some color for that cell, and try to solve the puzzle
 * in a memoized dfs fashion to the left and to the right, for every partition of the number of
 * black cells to the left and right, using the boundary constraint of the color we picked for c.
 *
 * Performance:
 * O(LK), runs the tests in 0.031s using 808KB memory.
 */

#include <stdio.h>
#include <memory>

char A[2][401][401][2]; // Memoizing over [dir][pos][black cells left][pos is black]
char E[401]; // Topology - start/end points
char S[401]; // Input array
char O[401]; // Output
int L, K, C; // C = number of black cells in total

bool dfs(bool f, int n, int k, int s) {
    // f = left or right, n = position in array, k = black cells left, s = this cell black
    int d = f ? 1 : -1;
    if(n == (f ? L : -1))
        return k == 0; // We read the entire array - did we use all black cells?
    if(A[f][n][k][s] >= 0) // Already memoed
        return A[f][n][k][s];
    if(s) { // This cell is black
        if(k == 0 || S[n] == '.') // Return if it's in fact white, or we don't have any black left
            return false;
        if(E[f ? C-k : k-1] & (f+1)) // If this is the end or beginning of a block
            return A[f][n][k][s] = dfs(f, n+d, k-1, 0); // Then next must be white
        return A[f][n][k][s] = dfs(f, n+d, k-1, 1); // Else black
    } else { // This cell is white
        if(S[n] == 'X') // If it's white in reality, fail
            return false;
        // Next cell can be either black or white
        return A[f][n][k][s] = dfs(f, n+d, k, 0) || dfs(f, n+d, k, 1);
    }
}

bool calc() {
    for(int n = 0; n < L; n++) { // For every cell
        bool w = false, b = false;
        // Can we paint this cell white?
        for(int k = 0; k <= C; k++) // For every number k of black squares
            if(k == C || E[C-k-1] & 2) // This can be white if prev black square wasn't an end
                w = w || dfs(1, n, k, 0) && dfs(0, n, C-k, 0); // Paint left and right
        // How about painting it black?
        for(int k = 0; k <= C; k++)
            b = b || dfs(1, n, k, 1) && dfs(0, n, 1+C-k, 1);
        if(w && !b) { // If we could be white but not black
            if(S[n] == 'X')
                return false;
            O[n] = '.';
        } else if(b && !w) { // Etc
            if(S[n] == '.')
                return false;
            O[n] = 'X';
        } else if(w && b) {
            O[n] = S[n];
        } else
            return false;
    }
    return true;
}

int main() {
    std::memset(A, 0xff, sizeof(A));

    int k;
    scanf("%d %d", &L, &K);
    for(int i = 0; i < K; i++, C += k)
        scanf("%d", &k), E[C] |= 1, E[C+k-1] |= 2; // Record if this is a start or end of a block
    scanf("%s", S);

    if(!calc())
        return printf("Impossible"), 0;
    printf("%s", O);
}
