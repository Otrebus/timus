/* 1277. Cops and Thieves - http://acm.timus.ru/problem.aspx?num=1277
 *
 * Strategy:
 * The minimum number of police necessary to guard the stations is the minimum cut of the graph
 * with respect to police control, which is the same as the maximum "flow" of police through the
 * stations. This number needs to be at least as big as the number of police, or a thief would
 * be able to slip by. To calculate the maximum flow we use a bastardization of Dinic's
 * algorithm where flow and capacity are associated with vertices rather than edges.
 *
 * Performance:
 * O(N^4), runs the tests in 0.015s using 280KB memory.
 */

#include <stdio.h>
#include <vector>
#include <queue>
#include <cstring>

std::vector<int> a[101]; // Adjacency lists
int c[101], f[101], l[101]; // Cost, flow, levels

int K, N, M, S, F, x, y;

int bfs(int node) {
    // Sets up the level graph
    std::queue<int> q;
    std::memset(l, 0, sizeof(l));
    q.push(node);
    l[node] = 1;
    while(!q.empty()) {
        int n = q.front();
        q.pop();
        for(auto v : a[n])
            if(c[v] > f[v] && !l[v])
                q.push(v), l[v] = l[n] + 1;
    }
    return l[F];
}

int dfs(int n, int flow) {
    // Finds some augmenting flow in the level graph
    if(n == F)
        return flow;
    int sum = 0;
    for(auto v : a[n]) {
        if(l[v] == l[n] + 1) {
            int fl = dfs(v, std::min(c[v]-f[v], flow));
            sum += fl;
            flow -= fl;
            f[v] += fl;
        }
    }
    return sum;
}

int main() {
    scanf("%d%d%d%d%d", &K, &N, &M, &S, &F);

    for(int i = 1; i <= N; i++)
        scanf("%d", &c[i]);

    c[S] = c[F] = 10000;

    for(int i = 1; i <= M; i++) {
        scanf("%d %d", &x, &y);
        a[x].push_back(y);
        a[y].push_back(x);
    }

    int sum = 0;
    if(F == S)
        return printf("NO"), 0;
    while(bfs(S))
        sum += dfs(S, 10000);
    printf(K < sum ? "NO" : "YES");
}
