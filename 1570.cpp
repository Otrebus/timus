/* 1570. Eating High - http://acm.timus.ru/problem.aspx?num=1570
 *
 * Strategy:
 * Dynamic programming over the filling value and the maximum index we are choose food from.
 *
 * Performance:
 * O((10^d)NM) where d is the number of decimal places of the filling value, runs the tests in
 * 0.015s using 392KB memory.
 */

#include <stdio.h>
#include <algorithm>

#pragma comment(linker, "/STACK:16777216")

struct dish {
    char n[31]; // Name
    int p, f, a; // Price, filling value, answer (used last)
} d[100];

struct ans {
    int i, n, p; // Food item, number of different items so far, price
};

ans A[20001][101];

int N, M;

ans dfs(int f, int n) {
    // Calculates the first optimal dish for a filling value, considering only dishes below index n
    if(f <= 0)
        return { -1, 0, 0 };
    if(A[f][n].p) // Already memoized
        return A[f][n];

    ans max = ans { -1, -1, 1000000000 };
    for(int i = std::min(n, N-1); i >= 0; i--) {
        auto a = dfs(f - d[i].f, i);
        // Decide what the optimal dish is by recursing; as tie breaker pick the one that
        // maximizes the number of different dishes
        if(d[i].p + a.p == max.p && a.n + (i != n) > max.n || d[i].p + a.p < max.p)
            max = { i, a.n + (i != n), d[i].p + a.p };
    }
    A[f][n] = max;
    return A[f][n];
}

int main() {
    scanf("%d %d", &N, &M);
    for(int i = 0; i < N; i++) {
        double x;
        scanf("%s %d %lf", &d[i].n, &d[i].p, &x);
        d[i].f = x*1000;
    }
    int f = M*1000;
    auto ans = dfs(f, N);
    printf("%d\n", ans.p);
    while(true) {
        // Construct the output from the given answer
        d[ans.i].a++;
        f-=d[ans.i].f;
        if(f <= 0)
            break;
        ans = A[f][ans.i];
    }
    for(int i = 0; i < N; i++)
        if(d[i].a)
            printf("%s %d\n", d[i].n, d[i].a);
}
