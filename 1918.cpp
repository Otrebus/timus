/* 1918. Titan Ruins: Artful Manipulations - http://acm.timus.ru/problem.aspx?num=1918
 *
 * Strategy:
 * The problem essentially asks for the number of sequences { x_1, ..., x_N } where for each
 * contiguous subsequence { x_1, .., x_k } where k < N, k < max { x_1, .., x_k } <= n. Let
 * A[n][i] denote the answer to the subproblem of the first n < N elements, where i is the
 * maximum element of that subsequence. Then, let C[n][i] be the sum of all C[n][l] where
 * 1 <= l <= i. We can then use dynamic programming to fill in the answers, bottom-up:
 * The recurrence (without modulo) is 
 *
 *   C[n][i] = C[n][i-1] + i(C[n-1][i]-C[n-1][i-1]) + C[n-1][i-1];
 *
 * for the subsolutions C[n-1][i-1], we add i to the end of those sequences to reach i as the 
 * maximum element, and for all the subsolutions C[n-1][i]-C[n-1][i-1], which are the solutions
 * which already have i as the maximum element, we can add any number. Then we also add C[n][i-1]
 * to make the answer cumulative.
 *
 * Performance:
 * O(N^2), runs the tests in 0.28s using 252KB memory.
 */

#include <stdio.h>
#include <array>

const int P = 1000000007, maxn = 5000;
long long C[2][maxn+1];

int main()
{
    int N;
    scanf("%d", &N);
    for(int i = 1; i <= N; i++)
        C[1%2][i] = i-1;
    for(int n = 2; n < N; n++)
    {
        C[n%2][n] = 0;
        for(int i = n + 1; i <= N; i++)
            C[n%2][i] = (C[n%2][i-1] + (P+C[(n-1)%2][i]-C[(n-1)%2][i-1])*i + C[(n-1)%2][i-1])%P;
    }
    printf("%lld", C[(N-1)%2][N]*N%P); // Last element can be any number up to N
}