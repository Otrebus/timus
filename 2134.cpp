/* 2134. Stampman - http://acm.timus.ru/problem.aspx?num=2134
 *
 * Strategy:
 * Store and apply only the painted coordinates of the stamp; since we can color at most n*m + k
 * coordinates before halting this bounds the execution time.
 *
 * Performance:
 * O(max(nm, q)), runs the tests in 0.062 seconds using 7,940KB memory.
 */

#include <stdio.h>

short X[1000000], Y[1000000]; // The coordinates of the painted stamp squares
int A[1001][1001]; // The document, in number of repaints
int p, r; // Number of painted stamp squares, total number of repainted squares of the document

int main() {
    int n, m, k, a, b, q, x, y;
    char s[1001]; // Each line of the stamp

    scanf("%d %d %d %d %d", &n, &m, &k, &a, &b);

    for(int i = 0; i < a; i++) { // Fill in the colored stamp coordinates
        scanf("%s", s);
        for(int j = 0; j < b; j++)
            if(s[j] == '#')
                X[p] = i, Y[p] = j, p++;
    }

    scanf("%d", &q);
    for(int i = 1; i <= q; i++) { // Update the document with the stamp
        scanf("%d %d", &x, &y);
        for(int j = 0; j < p; j++)
            if(A[X[j]+x][Y[j]+y]++ == 1 && ++r >= k) // Update the number of repaints and check
                return printf("Wasted after stamp #%d", i), 0;
    }

    for(int i = 1; i <= n; i++, putc('\n', stdout)) // Output the resulting document
        for(int j = 1; j <= m; putc(A[i][j++] ? '#' : '.', stdout));
}
