/* 2093. All Roads Lead to Snowdrift - http://acm.timus.ru/problem.aspx?num=2093
 *
 * Strategy:
 * Dijkstra, with a linear lookup of the cleaning schedules of a node's edges when it's relaxed.
 *
 * Performance:
 * O(m*log n + k), runs the tests in 0.281 seconds using 15,428KB memory.
 */

#include <stdio.h>
#include <vector>
#include <algorithm>
#include <set>
#include <cmath>

using ll = long long;
using ld = long double;
const int maxn = 1e5, maxm = 1e5;
const ll inf = std::numeric_limits<ll>::max();

struct edge {
    int i, n, t; // Index, destination, travel time
};

std::vector<std::pair<ll, ll>> C[maxm+1]; // Cleaning start/end times for each edge
int n, m, k, p, s, f;

struct node {
    ll t; // Min time so far
    bool v; // Visited
    std::vector<edge> e; // Edge list
} nodes[maxn+1];

ll calc(ll t, ll T) { // The time it takes to drive, from the description
    ld a = std::ceil((ld(T)/100 + 1)*t);
    if(100500.0*t < a)
        return 100500*t;
    return ll(a+0.1);
}

ll getT(ll t, edge e) { // Calculates the first instant we can pass edge e if arriving at time t
    ll ti = e.t, tp = 0;
    if(C[e.i].empty())
        return t + calc(ti, t);

    for(auto p : C[e.i]) {
        // For some cleaning that we can start driving before
        if(t <= p.first) {
            // Check that we finish driving before the cleaning starts
            ll t2 = t + calc(ti, t-tp);
            if(t2 <= p.first)
                return t2;
        }
        tp = p.second;
        t = std::max(t, p.second);
    }
    return t + calc(ti, t-tp);
}

ll solve() { // Dijkstra
    for(int i = 2; i <= n; i++)
        nodes[i].t = inf;

    std::set<std::pair<ll, int>> Q = {{0, 1}};

    while(!Q.empty()) {
        auto& p = *(Q.begin());
        auto t = p.first;
        auto& node = nodes[p.second];
        Q.erase(p);
        node.v = true;
        if(node.t != t)
            continue;

        for(auto& edge : node.e) {
            if(nodes[edge.n].v)
                continue;
            auto t2 = getT(t, edge);
            if(t2 < nodes[edge.n].t) {
                nodes[edge.n].t = t2;
                Q.erase( { nodes[edge.n].t, edge.n } );
                Q.insert( { t2, edge.n } );
            }
        }
    }
    return nodes[n].t;
}


int main() {
    scanf("%d %d", &n, &m);

    for(int i = 1; i <= m; i++) {
        int a, b, t;
        scanf("%d %d %d", &a, &b, &t);
        nodes[a].e.push_back( { i, b, t } );
        nodes[b].e.push_back( { i, a, t } );
    }
    scanf("%d", &k);

    for(int i = 0; i < k; i++) {
        scanf("%d %d %d", &p, &s, &f);
        C[p].push_back({ s, f });
    }
    for(int i = 1; i <= m; i++) // Sort the schedules
        std::sort(C[i].begin(), C[i].end(), [] (auto a, auto b) { return a.first < b.first; });
    printf("%lld", solve());
}
