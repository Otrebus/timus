/* 1805. Chapaev and a Cipher Grille - http://acm.timus.ru/problem.aspx?num=1805
 *
 * Strategy:
 * We assign the numbers in turn to the grille as follows: assume that the first i - 1 numbers have
 * been assigned, and we know that this grille is at rank m. In order to assign position i, we count
 * how many grilles start with the first i - 1 numbers that were already assigned followed by a zero
 * (for number i), by checking how many different ways we can assign each of the remaining positions
 * in the grille and multiplying these ways together. If the requested number k is greater than this
 * product, we must set position i to 1 in the grille. Otherwise, we set position i to 0, update m,
 * and continue with position i + 1.
 *
 * Performance:
 * O(n^4), runs the tests in 0.015s using 212KB memory.
 */

#include <stdio.h>

int A[101]; // The grille

int main()
{
    long long n, k;
    scanf("%lld %lld", &n, &k);

    long long m = 0;
    for(int i = 1; i <= n*n; i++) // For each position in the grille
    {
        long long M = 1;
        for(int x = 0; x < n/2; x++) // For each position in the upper left quadrant
        {
            for(int y = 0; y < n/2; y++)
            {
                int j = 1 + y*n + x, d = 0; // Count how many ways we can rotate this position still
                for(int r = 4; r; r--, j = (1 + (j - 1)%n)*n - (j - 1)/n) // Rotate the position
                    if(j < i && A[j])
                        r = d = 1; // Can't rotate since it was already assigned
                    else if(j > i)
                        d++;
                M *= d;
            }
        }
        if(k > m + M)
            m += M, A[i] = 1;
    }
    for(int y = 0; y < n; y++)
    {
        for(int x = 0; x < n; x++)
            printf("%d", A[1 + y*n + x]);
        printf("\n");
    }
}