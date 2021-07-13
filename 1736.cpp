/* 1736. Chinese Hockey - http://acm.timus.ru/problem.aspx?num=1736
 *
 * Strategy:
 * Dinic; construct a flow network where in the first layer each team has an incoming capacity from
 * the source equal to the number of points in play against teams of a higher index, which they
 * distribute in a second layer to the other teams, up to 3 points each, or to themselves,
 * limitlessly. Each team in the second layer then has an edge to the sink with its given score as
 * capacity.
 *
 * Performance:
 * O(n^4), runs the tests in 0.031s using 972KB memory.
 */

#include <stdio.h>
#include <vector>
#include <queue>

const int N = 200;

int C[2*N+2][2*N+2]; // Capacity
int l[2*N+2]; // Level

struct node {
    std::vector<int> v;
} nodes[2*N+2];


void add(int i, int j, int c) { // Adds the edge (i, j) with capacity c to the flow network
    nodes[i].v.push_back(j);
    nodes[j].v.push_back(i);
    C[i][j] = c;
}

int n;

int bfs() {
    // Sets up the level graph
    std::queue<int> q;
    std::memset(l, 0, sizeof(l));
    q.push(0);
    l[0] = 1;
    while(!q.empty()) {
        int u = q.front();
        q.pop();
        for(auto v : nodes[u].v)
            if(C[u][v] && !l[v])
                q.push(v), l[v] = l[u] + 1;
    }
    return l[2*n+1];
}

int dfs(int u, int flow) {
    // Finds some path with residual capacity and sends flow along it
    if(u == 2*n+1 || flow <= 0)
        return flow;
    int sum = 0;
    for(auto v : nodes[u].v) {
        if(l[v] == l[u] + 1) {
            int fl = dfs(v, std::min(flow, C[u][v]));
            sum += fl;
            flow -= fl;
            C[u][v] -= fl;
            C[v][u] += fl;
        }
    }
    return sum;
}

int main() {
    int k = 0, p;
    scanf("%d", &n);

    for(int i = 1; i <= n; i++) {
        add(0, i, 3*(n-i)); // Incoming points
        add(i, n+i, 3*(n-i)); // Points kept by the team
        for(int j = i + 1; j <= n; j++)
            add(i, n+j, 3); // Points given to other teams
        scanf("%d", &p);
        add(n+i, 2*n+1, p); // Points scored in total by the team (specified in the input)
    }
    
    while(bfs())
        dfs(0, 1e9);

    for(int i = n+1; i <= 2*n; i++)
        if(C[i][2*n+1]) // The flow to the sink by each team must match the specified score
            return printf("INCORRECT"), 0;

    k = 0;
    printf("CORRECT\n");
    // Print the results from how each team sends its points to the others
    const char* c[4] = { "<", "<=", ">=", ">" };
    for(int i = 1; i <= n; i++)
        for(int j = i+1; j <= n; j++)
            printf("%d %s %d\n", i, c[C[i][n+j]], j);
}

// An attempt where each node in the flow network is a game that receives 3 points and
// distributes them to the two teams, solved with Ford-Fulkerson. This is too slow though.
//   
//    #include <stdio.h>
//    #include <vector>
//    
//    const int N = 200;
//    
//    struct edge {
//        int u, c, r;
//    };
//    
//    struct node {
//        node() : p(-1) {}
//        std::vector<edge> v;
//        int p;
//    } nodes[2+N*(N-1)/2+N];
//    
//    
//    void add(int i, int j, int c) {
//        nodes[i].v.push_back({ j, c, (int)nodes[j].v.size() });
//        nodes[j].v.push_back({ i, 0, (int)nodes[i].v.size()-1 });
//    }
//    
//    int S, R;
//    
//    int dfs(int a) {
//        if(a == S)
//            return 3;
//        auto& node = nodes[a];
//        for(int i = 0; i < node.v.size(); i++) {
//            edge e = node.v[i];
//            auto& node2 = nodes[e.u];
//            if(e.c && node2.p < 0) {
//                node2.p = e.r;
//                int f = dfs(e.u);
//                if(f)
//                    return std::min(f, e.c);
//                node2.p = -1;
//            }
//        }
//        return 0;
//    }
//    
//    int main() {
//        int k = 0, n;
//        scanf("%d", &n);
//    
//        int s = n*(n-1)/2, l;
//        S = n*(n-1)/2+n+1;
//        for(int i = 1; i <= n; i++) {
//            for(int j = i+1; j <= n; j++) {
//                k++;
//                add(0, k, 3);
//                add(k, s+i, 3);
//                add(k, s+j, 3);
//            }
//        }
//        
//        for(int i = 1; i <= n; i++) {
//            scanf("%d", &l);
//            add(s+i, s+n+1, l);
//        }
//        int f = 0;
//        while(f = dfs(0)) {
//            for(int u = S; nodes[u].p >= 0;) {
//                edge& e = nodes[u].v[nodes[u].p];
//                edge& e2 = nodes[e.u].v[e.r];
//                e.c += f;
//                e2.c -= f;
//                nodes[u].p = -1;
//                u = e.u;
//            }
//        }
//    
//        for(auto u : nodes[0].v)
//            if(u.c)
//                return printf("INCORRECT"), 0;
//    
//        k = 0;
//        printf("CORRECT\n");
//        const char* c[4] = { "<", "<=", ">=", ">" };
//        for(int i = 1; i <= n; i++)
//            for(int j = i+1; j <= n; j++)
//                printf("%d %s %d\n", i, c[3-nodes[++k].v[1].c], j);    
//    }
