/* 1630. Talisman - http://acm.timus.ru/problem.aspx?num=1630
 *
 * Strategy:
 * We need the graph to consist only of components where we can spatially configure each component
 * such that every connected pair of nodes more than one edge apart is maximally separated (have the
 * connecting edges along a straight line). This is only possible in:
 * 
 *  a) Linear graphs
 *  b) Simplexes up to dimension three (complete graphs with at most 4 vertices).
 * 
 * We can find these by recording the maximum and minimum number of edges per node in each
 * component.
 * 
 * Performance:
 * O(M), runs the tests in 0.001s using 240KB memory.
 */

#include <vector>
#include <stdio.h>

std::vector<int> adj[101];
bool v[101];

struct st { int m, M, n; }; // Min/max number of edges per vertex in a component and number of verts

st dfs(int i) { // Records the above
    st r = { adj[i].size(), adj[i].size(), 0 };
    if(v[i])
        return r;
    v[i] = true;
    r.n++;

    for(auto v : adj[i]) {
        auto p = dfs(v);
        r.m = std::min(r.m, p.m);
        r.M = std::max(r.M, p.M);
        r.n += p.n;
    }
    return r;
}

bool check(int i) {
    if(v[i]) // Already visited
        return true;
    auto p = dfs(i);
    // Vertices in linear graphs all have two edges except for the endpoints, and vertices
    // in complete graphs of size n all have n-1 edges.
    return p.m == 1 && p.M == 2 || p.m == p.n-1 && p.M == p.n-1 && p.n < 5;
}

int main() {
    int N, M, x, y;
    scanf("%d %d", &N, &M);
    for(int i = 0; i < M; i++) {
        scanf("%d %d", &x, &y);
        adj[x].push_back(y), adj[y].push_back(x);
    }

    for(int i = 1; i <= N; i++)
        if(!check(i))
            return printf("Unlucky Petr"), 0;
    printf("Luck is possible");
}
