/* 1979. Resources Distribution - http://acm.timus.ru/problem.aspx?num=1979
 *
 * Strategy:
 * Have each side and its opposite side sum to 1+6*n^2, which we can do by having them equal i
 * and 1+6*n^2-i, respectively.
 *
 * Performance:
 * O(n^2), runs the tests in 0.015s using 356KB memory.
 */

#include <stdio.h>
#include <array>

int A[6][100][100];

int main() {
    int n;
    scanf("%d", &n);
    int s = 6*n*n, x = 1;
    for(int i = 0; i < 3; i++) // Assign the sides
        for(int j = 0; j < n; j++)
            for(int k = 0; k < n; A[i][j][k++] = x++)
                A[i+3][j][n-k-1] = 1+s-x;

    for(int j = 0; j < n; j++, printf("\n")) // Print them out
        for(int k = 0; k < n; printf("%d ", A[2][k++][j]));
    for(int j = 0; j < n; j++, printf("\n"))
        for(int i : { 0, 1, 3, 4 })
            for(int k = 0; k < n; printf("%d ", A[i][j][k++]));
    for(int j = 0; j < n; j++, printf("\n"))
        for(int k = 0; k < n; printf("%d ", A[5][k++][j]));
}
