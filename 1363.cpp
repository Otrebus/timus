/* 1363. Halftones - http://acm.timus.ru/problem.aspx?num=1363
 *
 * Strategy:
 * Sierra dithering.
 *
 * Performance:
 * O(MN), runs the tests in 0.062s using 1,932KB memory
 */

#include <stdio.h>

using ld = long double;

ld D[3][5] = { // The dithering matrix
    { 0, 0, 0, 5, 3 },
    { 2, 4, 5, 4, 2 },
    { 0, 2, 3, 2, 0 }
};

ld A[300][300]; // The input
int B[300][300]; // The output
ld E[300][300]; // Accumulated error
int N, M;

void apply(ld e, int x, int y) {
    // Applies the dithering matrix with the given error to E
    for(int i = -2; i <= 2; i++)
        for(int j = 0; j <= 2; j++)
            if(x+i < M && x + i >= 0 && y+i < N)
                E[x+i][y+j] += D[j][2+i]*e/32;
}

int main() {
    scanf("%d %d", &N, &M);
    for(int y = 0; y < N; y++)
        for(int x = 0; x < M; x++)
            scanf("%lf", &A[x][y]);

    for(int y = 0; y < N; y++) {
        for(int x = 0; x < M; x++) {
            B[x][y] = (A[x][y] + E[x][y] > 255.0/2) ? 255 : 0; // Output = Digitize(input + error)
            ld e = (A[x][y] + E[x][y]) - B[x][y]; // Error = Expected output - Digitized output
            apply(e, x, y); // Smear the error to the right and downwards with dithering
        }
    }
    
    for(int y = 0; y < N; y++, printf("\n"))
        for(int x = 0; x < M; x++)
            printf("%d ", B[x][y]);
}
