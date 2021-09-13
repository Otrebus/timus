/* 1526. Martian Plates - http://acm.timus.ru/problem.aspx?num=1526
 *
 * Strategy:
 * We can think of the problem as that of creating a string of balanced colored parentheses, where
 * some colored opening parentheses prevent other colors from being used in the enclosed substring.
 * Use dynamic programming over the colors used so far and the number of parentheses left to use.
 *
 * Performance:
 * O(tn2^n), runs the tests in 0.046s using 968KB memory.
 */

#include <stdio.h>
#include <memory>

// We differentiate between strings enclosed in outer parentheses like (...), vs not (..)(..)..
int A[2][101][1<<10]; // [enclosed][parentheses left to use][colors used]
int B[10]; // The colors forbidden 
int C[1<<10]; // The colors permitted by a set of given colors

int main() {
    int m, i, j, p, t, N;
    scanf("%d %d %d %d", &p, &t, &N, &m);

    while(m--) {
        scanf("%d %d", &i, &j);
        B[i-1] |= (1 << (j-1));
    }

    for(int m = 0; m < (1 << N); m++) {
        C[m] = ~0;
        for(int i = 0; i < N; i++)
            if(m & (1 << i))
                C[m] &= ~B[i];
        A[0][0][m] = A[1][0][m] = 1;
    }

    for(int r = 1; r <= t/2; r++) {
        for(int m = 0, s = 0; m < (1 << N); m++, s = 0) { // Loop over used colors
            // Completely enclosed parentheses (..)
            for(int i = 0; i < N; i++) // Loop over new colors used
                if(C[m] & (1 << i)) // If color is allowed according to the colors we picked
                    s = (s + A[0][r-1][m | (1 << i)]) % p; // Enclose, reuse inner n-1 result
            A[1][r][m] = s;

            s = 0; // Parenthesis trains (..)(..)
            for(int i = 0; i < r; i++)
                // Any enclosed string followed by any string expression (to not double count)
                s = (s + A[1][r-i][m]*A[0][i][m]) % p;
            A[0][r][m] = s;
        }
    }
    printf("%d", A[0][t/2][0]);
}
