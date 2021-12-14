/* 1696. Salary for Robots - http://acm.timus.ru/problem.aspx?num=1696
 *
 * Strategy:
 * Dynamic programming. Consider a graph of all possible salaries on the y axis, and each robot on
 * the horizontal axis. Build the answer by iterating over each robot and memoizing the solution
 * up to and including that robot by A[k][s] where k is the maximum salary encountered and s is
 * the maximum second descending salary encountered. To update A[k'][s'] for a new robot, we can
 * arrive at this value either by, from all previous solutions, increasing the maximum value of
 * any of the previous maximums k < k', or by increasing the second descending salary from any of
 * the previous s < s'. These are two different sums, from k..k' and s..s' that we avoid iterating
 * to calculate by having two instantiations of A[k][s] be the cumulative value of A up to k and
 * s respectively.
 *
 * Performance:
 * O(nk^2), runs the tests in 0.421s using 784KB memory.
 */

#include <stdio.h>
#include <algorithm>

unsigned int A[2][2][201][201], ans, N, K, P, p = 0, q = 1;

int main() {
    scanf("%u %u %u", &N, &K, &P);
    for(int k = 1; k <= K; k++) // Initialize by pretending a single salary of 1 at "robot #0"
        A[0][0][k][1] = 1;

    for(; N--; std::swap(p, q)) {
        // Calculate the cumulative answer over s (by adding the two sums described in the header)
        for(int k = 1; k <= K; k++) // Fix k
            for(int s = 1; s < k; s++) // Iterate s up to k
                A[q][1][k][s] = (A[q][1][k][s-1] + A[p][0][k][s] + A[p][1][k][s])%P;

        // Calculate the cumulative answer over k
        for(int s = 1; s <= K; s++) // Fix s
            for(int k = s; k <= K; k++) // Iterate k from s up to the maximum salary
                A[q][0][k][s] = (A[q][0][k-1][s] + A[p][0][k][s] + A[p][1][k][s])%P;
    }

    for(int s = 1; s <= K; s++)
        ans = (ans + A[p][0][K][s])%P;
    printf("%u", ans+1);
}
