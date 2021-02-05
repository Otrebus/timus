/* 1580. Dean's Debts - http://acm.timus.ru/problem.aspx?num=1580
 *
 * Strategy:
 * If we perform repeated substitutions on a system of equations
 * 
 *   x1 + x2 = A
 *   x2 + x3 = B
 *   x3 + x1 = C
 *      ...
 * 
 * we get x1 = A - x2 = A - (B - x3) = A - (B - (C - x1)), etc. If we treat every equation as the
 * edge in a graph with the variables as nodes we find that we will be able to resolve a variable
 * exactly when we have a loop with an odd number of vertices like in the example (otherwise
 * the variable cancels out on both sides and the equation is indeterminate). We find such a loop
 * with DFS. We can then start another variable substitution (DFS) at that variable and assign
 * values to every connected (by equations/edges) varible, using another DFS. Repeat this for every
 * component of the graph.
 *
 * Performance:
 * Linear in the size of the input, runs the tests in 0.125s using 15,384KB memory.
 */

#include <iostream>
#include <vector>
#include <iomanip>

struct edge { int c, n; }; // Edge, describing the rest of the equation (constant and other index)

struct node {
    int d; // The depth of the search to keep track of parity
    bool v; // Visited?
    int c; // The value of the constant (A - (B - ... )) in the equation at this point in the DFS
    std::vector<edge> e;
} nodes[1001];

bool v[1001][1001], w[1001][1001]; // Edges visited

int findLoop(node& n, int d, int c) {
    n.c = c, n.d = d;
    int l = 0, i = &n - nodes;

    for(auto& e : n.e) {
        node& n2 = nodes[e.n];
        int j = &n2-nodes;
        
        if(v[i][j])
            continue;
        v[i][j] = v[j][i] = true;

        if(n2.d && (d - n2.d)%2 == 0) {
            // If we find an odd loop, return the index and assign the constant here, which is
            // twice the value of the variable of the given index
            nodes[l=e.n].c = -c + e.c + n2.c;
            return l;
        }
        if(l = findLoop(n2, d + 1, -c + e.c))
            return l;
    }
    return 0;
}

bool calc(node& n, int c) {
    int i = &n - nodes;
    if(n.v && n.c != c) // Already visited, make sure the computed value is consistent
        return false;

    n.c = c, n.v = true;

    for(auto& e : n.e) {
        node& n2 = nodes[e.n];
        int j = &n2-nodes;

        if(w[i][j])
            continue;
        w[i][j] = w[j][i] = true;

        // Substitute the equation so far into the connected variable - here we double the constants
        // to avoid a division in the consistency check. We divide this later
        if(!calc(n2, -c + 2*e.c))
            return false;
    }
    return true;
}

int main() {
    int N, M;
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::cin >> N >> M;

    for(int i = 0; i < M; i++) {
        int x, y, A;
        std::cin >> x >> y >> A;
        nodes[x].e.push_back(edge { A, y });
        nodes[y].e.push_back(edge { A, x });
    }

    for(int i = 1; i <= N; i++) {
        if(nodes[i].v)
            continue;

        auto l = findLoop(nodes[i], 1, 0);

        if(!l || !calc(nodes[l], nodes[l].c)) {
            std::cout << "IMPOSSIBLE";
            return 0;
        }
    }

    for(int i = 1; i <= N; i++)
        std::cout << std::fixed << std::setprecision(2) << (nodes[i].c/2.0) << std::endl;
}
