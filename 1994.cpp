/* 1994. The Emperor's plan - http://acm.timus.ru/problem.aspx?num=1994
 *
 * Strategy:
 * Dynamic programming; see comments below for more details.
 *
 * Performance:
 * O(KN^3), runs the test suite in 0.031s using 364KB memory.
 */

#include <algorithm>
#include <stdio.h>

const int maxn = 200;
const int maxk = 20;

double P[maxn+1][(maxn+1)/2+1];
double A[maxn+1][maxk+1];

double C(int n, int k)
{
    return k > n ? 0 : P[n][std::min(k, n-k)];
}

int main()
{
    // Build Pascals triangle
    P[0][0] = 1;
    for(int n = 1; n <= maxn; n++)
    {
        P[n][0] = 1;
        for(int k = 1; k <= n/2; k++)
            P[n][k] = (P[n-1][k-1] + P[n-1][std::min(k, n-1-k)]);
    }
    int N, K;
    scanf("%d %d", &N, &K);
    N -= K;
    for(int i = 1; i <= maxn; i++)
        A[i][0] = i; // The expected number of remaining senators when all i senators are honest = i
    for(int n = 1; n <= N; n++)
    {
        for(int k = 1; k <= K; k++)
        {
            double max = 0;
            for(int d = 1; d <= n; d++) // The number of senators to kill
            {
                // Calculate the expected value; i is the number of honest senators that get killed
                double res = 0;
                // Each term is the number of ways to choose i from the honest senators, multiplied
                // by the number of ways to choose the remaining d-i from the spy senators
                // multiplied by the answer calculated earlier for the number of senators left once
                // we and the spies have had their way overnight
                for(int i = std::max(0, d-k); i <= std::min(d, n-k); i++)
                    res += C(n-k, i)*C(k, d-i)*A[n-k-i][k-(d-i)];
                // Finally we divide over all possible ways to choose d from n senators which is the
                // size of the "universe" in the probability calculation for the expected value
                res /= C(n, d);
                max = std::max(max, res);
            }
            A[n][k] = max;
        }
    }
    printf("%.10lf", A[std::max(0,N)][K]);
}
