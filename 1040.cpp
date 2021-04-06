/* 1040. Airline Company - http://acm.timus.ru/problem.aspx?num=1040
 *
 * Strategy:
 * Iterate the graph depth-first, assigning each traversed edge an incrementing number. This
 * guarantees that each node of degree > 1 encountered will get two edges with numbers k and k+1,
 * meaning the gcd of its edges will be 1.
 *
 * Performance:
 * O(M), runs the tests in 0.015s using 220KB memory.
 */

#include <vector>
#include <stdio.h>

struct node {
    std::vector<int> adj; // Adjacent nodes
    bool v; // Visited?
} nodes[51];

int e[51][51]; // The output
int i = 1;     // The variable we increment as we dfs through the tree

void dfs(int u) {
    if(nodes[u].v)
        return;
    nodes[u].v = true;
    for(auto v : nodes[u].adj) {
        if(!e[u][v]) // If we haven't assigned this edge already, do so
            e[u][v] = e[v][u] = i++;
        dfs(v); // Immediately dfs through that edge
    }
}

int main() {
    int N, M, x, y;
    scanf("%d %d", &N, &M);
    std::vector<std::pair<int, int>> edges;

    for(int i = 0; i < M; i++) {
        scanf("%d %d", &x, &y);
        edges.push_back({x, y});
        nodes[x].adj.push_back(y);
        nodes[y].adj.push_back(x);
    }

    dfs(1);

    printf("YES\n");
    for(int i = 0; i < M; i++)
        printf("%d ", e[edges[i].first][edges[i].second]);
}
