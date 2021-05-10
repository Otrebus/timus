/* 1905. Travel in Time - http://acm.timus.ru/problem.aspx?num=1905
 *
 * Strategy:
 * Depth-first search where we mark the last time we visited a node, and allow ourselves to
 * revisit a node whenever we arrive to that node early enough to use a Valkyrie that we were
 * unable to use prior.
 *
 * Performance:
 * O(m), runs the tests in 0.406s using 13,272KB memory.
 */

#include <iostream>
#include <vector>
#include <algorithm>

#pragma comment(linker, "/STACK:16777216")

struct edge {
    int n, t1, t2, i; // Destination, times, input index
    bool operator< (const edge& b) { return t1 < b.t1; }
};

struct node {
    node() : t(1e9), v({}) {}
    std::vector<edge> v; // Outgoing flights
    int t; // Time previously visited
} nodes[100001];

int n, m, a, b, ts, te;

std::vector<int> ans; // To build the predecessor path when we reach our target

bool dfs(int n, int t) {
    auto& nod = nodes[n];
    auto& v = nod.v;
    int pt = nod.t;

    if(n == b && t <= te) // Win condition
        return true;

    if(t >= pt) // Already visited later than this
        return false;
    nod.t = t;

    // Take all the flights later than our visit but earlier than the previous visit
    for(auto p = std::lower_bound(v.begin(), v.end(), edge{0, t}); p < v.end() && p->t1 < pt; p++) {
        if(dfs(p->n, p->t2)) {
            ans.push_back(p->i);
            return true;
        }
    }
    return false;
}

int main() {
    std::cin >> n >> m;

    for(int i = 1; i <= m; i++) {
        int x, y, t1, t2;
        std::cin >> x >> y >> t1 >> t2;
        nodes[x].v.push_back( { y, t1, t2, i } );
    }
    std::cin >> a >> b >> ts >> te;
    for(int i = 1; i <= n; i++)
        std::sort(nodes[i].v.begin(), nodes[i].v.end()); // For binary search when we reach the node

    if(!dfs(a, ts)) {
        std::cout << "Impossible";
        return 0;
    }

    std::cout << ans.size() << std::endl;
    for(int i = ans.size()-1; i >= 0; i--)
        std::cout << ans[i] << " ";
}
