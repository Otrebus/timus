/* 2119. Tree Hull - http://acm.timus.ru/problem.aspx?num=2119
 *
 * Strategy:
 * The weight of the smallest spanning tree equals the sum of all w[v] - lca(v, v+1) where v are
 * the vertices of the tree sorted in a depth-first order (n+1 = 1) and w[v] is the sum of the
 * weights from the root to v and lca is the least common ancestor. When adding and removing
 * vertices from the tree we maintain the dfs order by using an ordered set.
 *
 * Performance:
 * Q((q+n)log n), runs the tests in 1.953s using 90,576KB memory.
 */

#include <stdio.h>
#include <vector>
#include <set>

#pragma comment(linker, "/STACK:64777216")

const int maxn = 3e5;
using ll = long long;

int e = 1, t = 0;
ll ans;
std::set<int> s;
int R[maxn]; // Maps depth-first order to node index

struct node {
    std::vector<int> p; // k^2th ancestors
    std::vector<std::pair<int, int>> v; // Children
    std::pair<int, int> e; // Enter time, leave time
    int t; // Depth-first order
    ll w; // Sum of weight
} N[maxn+1];

void dfs(int i, int p, ll s, int d, std::vector<int>& v) {
    N[i].t = t++; // The dfs ordering
    R[N[i].t] = i;

    // Assign ancestors (could also have walked up the ancestor arrays instead of using v)
    N[i].p.push_back(i);
    for(int h = 1; h <= d; h *= 2)
        N[i].p.push_back(v[v.size()-h]);

    N[i].e.first = e++;
    N[i].w = s;

    // Recurse
    v.push_back(i);
    for(auto u : N[i].v)
        if(u.first != p)
            dfs(u.first, i, s + u.second, d+1, v);
    v.pop_back();
    N[i].e.second = e++;
}

int lca(int a, int b) {
    // Picks b as path to the root, and runs up the ancestor array of a in lg time to check when
    // we first enter the path of b
    int anc = b;

    for(int j = N[b].p.size()-1; j >= 0; j--) {
        auto q = N[b].p[j];
        if(N[q].e.first <= N[a].e.first && N[q].e.second >= N[a].e.second) // Ancestor check
            anc = q;
        else if(b != q)
            b = q, j = N[b].p.size();
    }
    return anc;
}

ll diff(int i) {
    // Calculates the difference of the sum mentioned in the header if we add i
    // to the set of vertices
    if(s.empty())
        return 0;

    // The neighboring nodes to i
    auto ita = --s.lower_bound(i);
    auto itb = s.upper_bound(i);

    ita = ita == s.end() ? --ita : ita;
    itb = itb == s.end() ? s.begin() : itb;

    auto a = R[*ita], b = R[*itb];
    return N[R[i]].w + N[lca(a, b)].w - N[lca(a, R[i])].w - N[lca(R[i], b)].w;
}

void insert(int i) {
    ans += diff(N[i].t);
    s.insert(N[i].t);
}

void remove(int i) {
    s.erase(N[i].t);
    ans -= diff(N[i].t);
}

int main() {
    int n, u, v, w, q;
    char t[2];
    scanf("%d", &n);
    for(int i = 0; i < n-1; i++) {
        scanf("%d %d %d", &u, &v, &w);
        N[u].v.push_back( { v, w } );
        N[v].v.push_back( { u, w } );
    }

    std::vector<int> va; // We keep the current path from the root here, for ancestors
    dfs(1, 0, 0, 0, va);

    scanf("%d", &q);
    while(q--) {
        scanf("%s %d", &t, &v);
        t[0] == '+' ? insert(v) : remove(v);
        printf("%lld\n", ans);
    }
}
