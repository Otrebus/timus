/* 1533. Fat Hobbits - http://acm.timus.ru/problem.aspx?num=1533
 *
 * Strategy:
 * We're asked to find a maximal antichain of a poset, so we use Kőnig's theorem to find the
 * largest antichain as the complement of the minimum vertex cover of the bipartite graph that
 * represents the poset.
 *
 * Performance:
 * Runs the tests in 0.015s using 652KB memory.
 */

#include <vector>
#include <stdio.h>

const int maxn = 101, inf = 1e6;
const int mn = maxn*2+2;

struct node {
    bool b; // Visited during dfs
    std::vector<int> v; // Neighbors
} nodes[mn];

int C[mn][mn]; // Capacity matrix
int F[mn][mn]; // Flow matrix
bool V[mn]; // Visited

int A[maxn][maxn];
int N, p = 1;

void add(int a, int b) { // Links a pair of nodes in the flow graph
    nodes[a].v.push_back(b);
    nodes[b].v.push_back(a);
    C[a][b] = 1;
}

bool dfs(int u) { // Finds an augmenting path
    nodes[u].b = true;
    if(u == N*2+1) // End node found
        return true;
    for(auto v : nodes[u].v)
        if(!nodes[v].b && C[u][v] > 0 && dfs(v)) {
            F[u][v]++, F[v][u]--, C[u][v]--, C[v][u]++;
            return true;
        }
    return false;
}

bool L[mn], R[mn], L2[mn]; // Unmatched vertices on the left + vertices reached by alternate paths

void find(int u, bool b) {
    // Finds an alternating path from unmatched vertices
    if(!u || u == 2*N+1 || nodes[u].b)
        return;
    nodes[u].b = true;
    for(auto v : nodes[u].v) {
        if(b && F[u][v] < 1 || !b && F[u][v] == -1 ) {
            if(v > N)
                R[v-N] = true;
            if(v <= N)
                L2[v] = true; 
            find(v, !b);
        }
    }
}

int main() {
    scanf("%d", &N);
    for(int i = 1; i <= N; i++)
        for(int j = 1; j <= N; j++)
            scanf("%d", &A[i][j]);

    // Form the transitive closure
    for(int k = 1; k <= N; k++)
        for(int i = 1; i <= N; i++)
            for(int j = 1; j <= N; j++)
                if(A[i][k] && A[k][j])
                    A[i][j] = 1;

    // Form the bipartite graph that expresses the poset, every relation a < b is an edge in a
    // bipartite graph, and we find a maximum matching. First, add the source edges
    for(int i = 1; i <= N; i++)
        add(0, i);

    // Add the middle edges, the relations
    for(int i = 1; i <= N; i++)
        for(int j = 1; j <= N; j++)
            if(i != j && A[i][j])
                add(i, N+j);

    // Add the sink edges
    for(int i = 1; i <= N; i++)
        add(N+i, 2*N+1);

    // Employ Ford-Fulkerson to find a maximum matching
    for(int aug = true; aug; aug = dfs(0))
        for(int i = 0; i < 2+N*2; i++)
            nodes[i].b = false;

    // Mark matched edges on the left
    for(int i = 1; i <= N; i++)
        for(int j = 1; j <= N; j++)
            if(F[i][N+j])
                L[i] = true;

    // Here we find the constituent parts of (A - Z) | (B & Z) where A and B are the bipartite sets
    // and Z are the vertices that we can reach by alternating paths from unmatched vertices on the
    // left bipartite set; this is the minimum vertex cover by the theorem mentioned in the header
    // comment
    for(int i = 0; i < 2*N+2; i++)
        nodes[i].b = false;
    for(int i = 1; i <= N; i++)
        if(!L[i])
            find(i, true);

    // Take the complement of the above to find the maximum antichain
    std::vector<int> out;
    for(int i = 1; i <= N; i++)
        if((!L[i] || L2[i]) && !R[i])
            out.push_back(i);

    printf("%d\n", out.size());
    for(auto o : out)
        printf("%d ", o);
}
