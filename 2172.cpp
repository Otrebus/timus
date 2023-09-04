/* 2172. Meta-statement 2 - http://acm.timus.ru/problem.aspx?num=2172
 *
 * Strategy:
 * Naive. Calculate M as max(ceil(Σs_i/K), max{s_i}), then cyclically distribute the input.
 *
 * Performance:
 * O(Σs_i), runs the tests in 0.078s using 3,684KB memory.
 */

#include <stdio.h>
#include <vector>

int N, K;
int A[1000];

int main() {
    scanf("%d %d", &N, &K);

    int sum = 0, max = 0;
    for(int i = 0; i < N; i++) {
        scanf("%d", &A[i]);
        sum += A[i];
        max = std::max(A[i], max);
    }

    // M has to be at least the ceiling of sum/K or max{s_i}
    int M = std::max((K-1+sum)/K, max);
    std::vector v(M, std::vector<int>());
    for(auto& x : v) v.reserve(K);

    // Cyclically distribute
    for(int i = 0, a = 0; i < N; i++)
        for(int j = 0; j < A[i]; j++)
            v[a++%M].push_back(i);

    printf("%d\n", M);
    for(int i = 0; i < M; i++) {
        printf("%d ", v[i].size());
        for(auto x : v[i])
            printf("%d ", x+1);
        printf("\n");
    }
}
