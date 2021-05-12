/* 1472. Martian Army - http://acm.timus.ru/problem.aspx?num=1472
 *
 * Strategy:
 * Since we are dealing with a piecewise linear function over the A_i's, we only need to consider
 * values at the extremal points (0 and 1), so for each node we recursively calculate and store
 * the least cost of its subtree for both of its reliability value extremals.
 * 
 * Performance:
 * O(N), runs the tests in 0.062s using 10,700KB memory.
 */

#include <stdio.h>
#include <vector>
#include <algorithm>

#pragma comment(linker, "/STACK:16777216")

using ld = long double;

struct edge { int k, c; }; // Destination, cost
struct {
    std::vector<edge> v;
    int p[2]; // Least cost assuming we have reliability 0 or 1
} nodes[100001];

int dfs(int i, int A) { // Returns the least cost of this subtree assuming root has reliability A
    auto& node = nodes[i];
    auto& p = node.p;

    if(A && node.v.empty()) // Leafs can't have any reliability
        return 1e9;
    if(p[A] > -1)
        return p[A];

    int c = 0;
    for(auto& e : node.v) {
        // Update our least cost with both possible reliability values for each node
        int c0 = e.c*A + dfs(e.k, 0);
        int c1 = e.c*std::max(A-1, 0) + dfs(e.k, 1);
        c += std::min(c0, c1);
    }
    return p[A] = c;
}

int main() {
    int N;
    scanf("%d", &N);

    for(int i = 2; i <= N; i++) {
        int K, C;
        scanf("%d %d", &K, &C);
        nodes[K].p[0] = nodes[K].p[1] = -1;
        nodes[K].v.push_back({i, C});
    }

    printf("%d.00", dfs(1, 1));
}

