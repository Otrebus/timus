/* 1076. Trash - http://acm.timus.ru/problem.aspx?num=1076
 *
 * Strategy:
 * Min-cost max-flow using spfa.
 *
 * Performance:
 * Not sure of the complexity, runs the tests in 0.031s using 852KB memory.
 */

#include <vector>
#include <queue>
#include <iostream>

const int maxv = 2+150*2;
int c[maxv][maxv], d[maxv], p[maxv]; // Edge cost, distance, parent
bool iq[maxv]; // Whether node is in the spfa queue
bool V[maxv][maxv]; // Whether a path is augmentable
std::vector<int> adj[maxv]; // Adjacency matrix

int n, nv, x;
const int inf = 0x3fffffff;

void spfa() {
    // Standard shortest-path faster algorithm
    for(int i = 0; i < nv; i++)
        d[i] = inf, p[i] = 0, iq[i] = false;
    d[0] = 0, p[0] = -1;
    std::queue<int> q;
    q.push(0);

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        iq[u] = false;
        for (int v : adj[u]) {
            int k = d[u] + c[u][v];
            if (V[u][v] && d[v] > k) {
                d[v] = k, p[v] = u;
                if (!iq[v])
                    iq[v] = true, q.push(v);
            }
        }
    }
}

int solve() {
    // Runs the spfa algorithm until there are no longer any augmenting flows
    int c = 0, t = nv-1;
    for(spfa(); d[t] != inf && (c += d[t], 1); spfa())
        for(int cur = t; cur; cur = p[cur])
            V[p[cur]][cur] = 0, V[cur][p[cur]] = 1;
    return c;
}

void add(int u, int v) {
    // Adds the given edge to the flow network
    adj[u].push_back(v), adj[v].push_back(u);
    V[u][v] = 1;
}

int main() {
    std::cin >> n;
    nv = 2*n + 2; // Number of vertices in the flow network

    for(int i = 0; i < n; i++) {
        // Set up the flow network
        add(0, 1+i);
        add(1+i+n, 1+2*n);

        for(int j = 0; j < n; j++) {
            std::cin >> x;
            for(int k = 0; k < n; k++)
                if(k != i) // Cost of moving trash to this bin = amount of trash in the other bins
                    c[1+k][n+1+j] += x, c[n+1+j][1+k] -= x;
            add(1+i, n+1+j);
        }
    }

    std::cout << solve();
}
