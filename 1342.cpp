/* 1342. Enterprise - http://acm.timus.ru/problem.aspx?num=1342
 *
 * Strategy:
 * Dynamic programming where we use subsolutions for using the first n workshops making
 * a total of m brooms - see comments below.
 * 
 * Performance:
 * O(NMK), with K being the maximum number of brooms any workshop can produce, runs the
 * tests in 0.281s using 8,300KB memory.
 */


#include <iostream>
#include <cmath>
#include <cstdio>
#include <iomanip>
#include <array>

struct {
    int k;
    double p, q, r;
} W[1001];

int S[1001];
double C[1001][1001]; // The subsolutions

double inf = std::numeric_limits<double>::infinity();

int main() {

    int N, M;

    scanf("%d %d", &N, &M);

    for(int i = 1; i <= N; i++) {
        scanf("%d %lf %lf", &W[i].k, &W[i].p, &W[i].q);
        W[i].r = 0.5*(W[i].q - W[i].p)/double(W[i].k-1.0); // Calculates part of the integral ahead
        S[i] = S[i-1] + W[i].k; // The number of brooms all workshops up to this can produce
    }

    for(int n = 0; n <= N; n++)
        for(int m = 0; m <= M; m++)
            C[m][n] = inf;
    C[0][0] = 0;

    for(int n = 1; n <= N; n++) {
        for(int m = std::max(std::min(W[n].k, M - (S[N] - S[n])), 0); m <= std::min(S[n], M); m++) {
            // The max total production in workshops up to n depends on how much we need in the
            // rest of the workshops, and how much we can actually produce
            double min = inf;
            for(int i = std::max(m - S[n-1], 0); i <= std::min(W[n].k, m); i++)
                // For each number of brooms i that makes sense to produce in the workshop, update
                // the optimal value of C[m][n] given C[m-i][n-1]
                min = std::min(C[m-i][n-1] + W[n].p*i + (i <= 1 ? 0.0 : i*(i-1.0)*W[n].r), min);
            C[m][n] = min;
        }
    }

    std::cout << std::fixed << std::setprecision(2);

    if(C[M][N] == inf) {
        while(C[M][N] == inf)
            M--;
        std::cout << "Maximum possible amount: " << M << std::endl;
    }
    std::cout << "Minimum possible cost: " << C[M][N];
}   
