/* 1584. Pharaohs’ Secrets - http://acm.timus.ru/problem.aspx?num=1584
 *
 * Strategy:
 * Min-cost maximum flow. In the flow network, connect the source to a node for each rectangle
 * (each group of statues that can stand in the symmetric position) with an edge with a capacity
 * of 1 and a cost of 0, connect each rectangle to each letter (statue type) with an edge with
 * capacity 1 and cost 4-a where a is the number of such letters already in the rectangle,
 * and connect each letter to the sink with an edge with capacity b/4 and cost 0 where b is the
 * total number of such letters in the grid. Then solve the resulting flow network and report
 * the total cost.
 *
 * Performance:
 * Runs the tests in 0.001s using 264KB memory.
 */

#include <vector>
#include <queue>
#include <stdio.h>

const int nc = 'z'-'a'+1;
const int inf = 0x3fffffff;

const int maxv = 2+20*20/4+nc;
int c[maxv][maxv], d[maxv], p[maxv], cap[maxv][maxv]; // Cost, distance, parent, capacity
bool iq[maxv]; // Is node in queue?
char A[20][21]; // Input letters
int B[nc]; // Letter frequency
std::vector<int> adj[maxv]; // Adjacency lists

int n, m, nv, x;

void spfa() {
    // Shortest-paths faster, finds the cheapest augmenting path
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
            if (cap[u][v] && d[v] > k) {
                d[v] = k, p[v] = u;
                if (!iq[v])
                    iq[v] = true, q.push(v);
            }
        }
    }
}

int solve() {
    // Sends flow through the cheapest augmenting path until there are no more such paths
    int cost = 0, t = nv-1;
    for(spfa(); d[t] != inf; spfa())
        for(int cur = t; cur; cur = p[cur])
            cost += c[p[cur]][cur], cap[p[cur]][cur]--, cap[cur][p[cur]]++;
    return cost;
}

void add(int u, int v, int ca, int cost) {
    // Adds an edge to the flow network
    adj[u].push_back(v), adj[v].push_back(u);
    cap[u][v] = ca;
    c[u][v] = cost, c[v][u] = -cost;
}

int main() {
    scanf("%d %d", &n, &m);
    nv = m*n/4 + nc + 2;

    for(int i = 0; i < n; i++) {
        scanf("%s", A[i]);
        for(int j = 0; j < m; j++)
            B[A[i][j]-'a']++; // Add letter to frequency
    }

    for(int i = 0, k = 0; i < n/2; i++) {
        for(int j = 0; j < m/2; j++, k++) {
            add(0, k+1, 1, 0); // Add the edge from the source to the group
            for(char c = 'a'; c <= 'z'; c++) {
                int r = (A[i][j]!=c) + (A[n-i-1][j]!=c) + (A[n-i-1][m-j-1]!=c) + (A[i][m-j-1]!=c);
                add(k+1, n*m/4+1+c-'a', 1, r); // Add the edge from the group to the letter
            }
        }
    }
    for(int i = 0; i < nc; i++)
        add(n*m/4+1+i, n*m/4+1+nc, B[i]/4, 0); // Add the edge from each letter to each character

    printf("%d", solve());
}