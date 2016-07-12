/* 1443. Rails - http://acm.timus.ru/problem.aspx?num=1443
 *
 * Strategy:
 * Greedy. Cover the required railways with (S/L)*N rails until the last N S%L-sized stubs remain.
 * Then, each new track can be used to cover L/(S%L) stubs, so the amount of extra rails required
 * is N/(L/(S%L)), plus a final track if there is a remainder N%(L/(S%L)).
 *
 * Performance:
 * Constant, runs the test cases in 0.001s using 176KB memory.
 */

#include <stdio.h>

int main()
{
    int N; 
    double s, l;
    scanf("%d %lf %lf", &N, &s, &l);
    int S = (s+1e-8)*10000, L = (l+1e-8)*10000;
    printf("%d", S/L*N + (S%L ? N/(L/(S%L)) + bool(N%(L/(S%L))) : 0));
}