/* 1664. Pipeline Transportation - http://acm.timus.ru/problem.aspx?num=1664
 *
 * Strategy:
 * Dinic.
 *
 * Performance:
 * O(N^3), runs the tests in 0.468s using 2,756KB memory.
 */

#include <stdio.h>
#include <vector>
#include <queue>
#include <unordered_set>
#include <cstring>

int l[10001]; // Level
int N, M;

std::unordered_set<int> S; // Prevents outputting edges twice

struct edge {
    int u, c, r; // Outgoing node, capacity, index of reverse edge on u
};

struct node {
    std::vector<edge> v;
} nodes[10001];


void add(int i, int j, int c) {
    // Adds the edge (i, j) with capacity c to the flow network
    nodes[i].v.push_back({ j, c });
    nodes[j].v.push_back({ i, c });
    nodes[i].v.back().r = nodes[j].v.size() - 1;
    nodes[j].v.back().r = nodes[i].v.size() - 1;
}

int n;

int bfs() {
    // Sets up the level graph
    std::queue<int> q;
    std::memset(l, 0, sizeof(l));
    q.push(1);
    l[1] = 1;
    while(!q.empty()) {
        int u = q.front();
        q.pop();
        for(auto& v : nodes[u].v)
            if(v.c > 0 && !l[v.u])
                q.push(v.u), l[v.u] = l[u] + 1;
    }
    return l[N];
}

int dfs(int u, int flow) {
    // Finds some path with residual capacity and sends flow along it
    if(u == N || flow <= 0)
        return flow;
    int pre = flow;
    for(auto& v : nodes[u].v) {
        if(v.c > 0 && l[v.u] == l[u] + 1) {
            int fl = dfs(v.u, std::min(flow, v.c));
            flow -= fl;
            v.c -= fl;
            auto& v2 = nodes[v.u].v[v.r];
            v2.c += fl;
        }
    }
    return pre - flow;
}

int main() {
    std::vector<std::pair<int, int>> v;
    scanf("%d", &N);

    for(int i = 0; i < N; i++)
        scanf("%*d %*d");

    scanf("%d", &M);
    for(int i = 0; i < M; i++) {
        int A, B, C;
        scanf("%d %d %d", &A, &B, &C);
        v.push_back({A, B});
        add(A, B, C);
    }

    while(bfs()) // Dinic
        dfs(1, 2e9);

    int sum = 0;
    for(auto a : nodes[N].v) // The flow is the amount going into the sink
        sum += std::abs((a.c-nodes[a.u].v[a.r].c)/2);

    printf("%d\n", sum);
    for(int i = 1; i <= N; i++) {
        for(auto v : nodes[i].v) {
            if(S.find(v.u*10001 + i) != S.end()) // Already mentioned this edge
                continue;
            auto v2 = nodes[v.u].v[v.r]; // Reverse edge
            if(v.c > v2.c) // If flow goes forward, output accordingly
                printf("%d %d %d\n", v.u, i, (v.c-v2.c)/2);
            else
                printf("%d %d %d\n", i, v.u, (v2.c-v.c)/2);
            S.insert(i*10001 + v.u);
        }
    }
}
