/* 2149. Pigeonhole Principle - https://acm.timus.ru/problem.aspx?num=2149
 *
 * Strategy:
 * Naive.
 *
 * Performance:
 * O(n), runs the tests in 0.015s using 152KB memory.
 */

#include <stdio.h>
#include <algorithm>

int S[2][2], P[2][2], n;
char s[500];

int main() {
    scanf("%d %s %*s %*s", &n, &s);
    for(int i = 0; i < n; i++) {
        S[i<n/2][s[i*5] == '<']++;
        P[i%2][s[i*5] == '<']++;
    }
    int a = std::min(S[0][1] + S[1][0], S[0][0] + S[1][1]);
    int b = std::min(P[0][1] + P[1][0], P[0][0] + P[1][1]);
    printf("%d", std::min(a, b));
}
