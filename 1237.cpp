/* 1237. Evacuation Plan - http://acm.timus.ru/problem.aspx?num=1237
 *
 * Strategy:
 * Formulate the problem as a min-cost maximum-flow problem, find a negative-cost cycle with
 * residual capacity and send a flow of size 1 around the cycle.
 *
 * Performance:
 * O((N+M)^3), runs the tests in 0.031s using 664KB memory.
 */

#include <iostream>
#include <vector>

const int maxn = 101;

struct building { int x, y, w; } A[maxn];
struct shelter { int x, y, w; } B[maxn];

const int maxv = maxn*2+2, inf = 1000000;

std::vector<int> adj[maxv]; // Adjacency
// Cost, capacity, depth (for cycle finding), previous (same)
int C[maxv][maxv], c[maxv][maxv], d[maxv], p[maxv];

int N, M, L;

void add(int i, int j, int w) {
    adj[i].push_back(j), adj[j].push_back(i);
    c[i][j] = w;
}

int findLoop()
{   // Finds a node with some ancestor in a negative-cost cycle in the residual graph
    // (or -1 if no such node exists)
    int l;
    for(int i = 0; i < L; i++)
        d[i] = inf;
    d[0] = 0;
    for (int k = 0; k < L-1; k++) {
        l = -1;
        for (int i = 0; i < L; i++)
            for (auto j : adj[i])
                if (c[i][j] > 0 && d[i] + C[i][j] < d[j])
                    d[j] = d[i] + C[i][j], p[l=j] = i;
    }
    return l;
}

int main() {

    std::cin >> N >> M;
    L = 2 + N + M;

    // Read the input
    for(int i = 1; i <= N; i++)
        std::cin >> A[i].x >> A[i].y >> A[i].w;
    for(int i = 1; i <= M; i++)
        std::cin >> B[i].x >> B[i].y >> B[i].w;

    // Connect the buildings to the source
    for(int i = 1; i <= N; i++)
        add(i, 0, A[i].w);

    // Connect the shelters to the sink
    for(int j = 1; j <= M; j++)
        add(N+j, L-1, B[j].w);

    // Knit together buildings and shelters with the appropriate connections, cost and flow
    for(int i = 1; i <= N; i++) {
        for(int j = 1; j <= M; j++) {
            int x;
            std::cin >> x;
            add(i, N+j, inf);
            c[N+j][i] = x;
            int cost = std::abs(A[i].x-B[j].x) + std::abs(A[i].y-B[j].y) + 1;

            C[i][N+j] = cost;
            C[N+j][i] = -cost;
            
            c[N+j][L-1] -= x;
            c[L-1][N+j] += x;
        }
    }

    int x = findLoop(), v = 0; // Find a node with an ancestor in a negative cycle
    if(x == -1)
        return std::cout << "OPTIMAL", 0;

    for (int i = 0; i < L; ++i)
        v = x = p[x]; // Reverse the path until we are certain to have entered the cycle

    std::vector<int> l;
    do l.push_back(v), v = p[v]; while(v != x); // Produce the cycle
    for(int v : l) // Increase the flow in the cycle
        c[p[v]][v] -= 1, c[v][p[v]] += 1;
    
    std::cout << "SUBOPTIMAL" << std::endl;
    for(int i = 1; i <= N; i++) {
        for(int j = 1; j <= M; j++) {
            std::cout << c[N+j][i] << " ";
        }
        std::cout << std::endl;
    }
}
