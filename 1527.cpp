/* 1527. Bad Roads - http://acm.timus.ru/problem.aspx?num=1527
 *
 * Strategy:
 * For a given h, we solve the shortest-paths problem by: for each i from 0 up to min(n, b), run
 * Dijkstra only considering paths using at most i private roads, putting any new nodes we reach
 * with private edges into the priority queue of the i+1'th iteration. Run binary search on top
 * of this over h to find the minimum h that allows for a solution.
 *
 * Performance:
 * O(nmlog n), runs the tests in 0.046s using 832KB memory.
 */

#include <stdio.h>
#include <vector>
#include <set>

struct edge {
    int i, u, v, c, t, h;
} edges[10001];

struct node {
    std::vector<edge> e;
    int t[101], p[101]; // Minimal time and parent for each iteration
} nodes[101];

const int inf = 0x3f3f3f3f;
int n, m, s, f, b, t, h;

int solve(int h) { // Finds the shortest path time for a car with height h
    std::set<std::pair<int, int>> Q[101];

    for(int i = 1; i <= n; i++) // Reset all the costs and parents
        for(int j = 0; j <= std::min(n, b); j++)
            nodes[i].t[j] = inf, nodes[i].p[j] = 0;

    nodes[s].t[0] = 0;
    Q[0].insert( { 0, s } );
    for(int i = 0; i <= std::min(b, n); i++) {
        while(!Q[i].empty()) {
            // We do regular Dijkstra here except we put each new/relaxed node into either the
            // current queue if it's reachable by public road, or the next queue if it's reachable
            // by a private road
            auto& p = *(Q[i].begin());
            auto t = p.first;
            auto& node = nodes[p.second];
            Q[i].erase(p);

            for(auto& edge : node.e) {
                int j = i + edge.c;
                int t2 = nodes[edge.u].t[i] + edge.t;
                if(j <= std::min(b, n) && t2 < nodes[edge.v].t[j] && h >= edge.h) {
                    nodes[edge.v].t[j] = t2;
                    nodes[edge.v].p[j] = edge.i;
                    Q[j].insert( { t2, edge.v } );
                }
            }
        }
        Q[i].clear();
    }
    for(int i = 0; i <= std::min(n, b); i++)
        if(nodes[f].t[i] <= t)
            return nodes[f].t[i];

    return inf;
}

int binary(int l, int r) { // Binary search
    while(true) {
        int mid = (r+l)/2;
        int result = solve(mid);
        if(l>=r)
            return mid;
        if(result > t)
            l = mid+1;
        else
            r = mid;
    }
}

int main() {
    scanf("%d %d %d %d %d %d", &n, &m, &s, &f, &b, &t);

    // Read the input, build the graph
    for(int i = 1; i <= m; i++) {
        int u, v, c, t, h;
        scanf("%d %d %d %d %d", &u, &v, &c, &t, &h);
        edges[i] = { i, u, v, c, t, h };
        nodes[u].e.push_back(edges[i]);
    }
    int r = binary(0, 10e6);
    if(r >= 10e6)
        return printf("-1"), 0;

    // Reconstruct the best path from the parent info
    std::vector<int> out;
    int j = -1;
    for(int i = 0; i <= std::min(b, n); i++)
        if(nodes[f].t[i] <= t)
            j = i;
    for(int i = f; i != s;) {
        auto p = nodes[i].p[j];
        out.push_back(p);
        i = edges[p].u;
        if(edges[p].c)
            j--;
    }
    printf("%d\n%d\n", r, out.size());
    for(auto it = out.rbegin(); it < out.rend(); it++)
        printf("%d ", *it);
}
