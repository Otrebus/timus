/* 2132. Graph Decomposition. Version 2 - http://acm.timus.ru/problem.aspx?num=2132
 *
 * Strategy:
 * The proof given in 1320.cpp can be used to construct a solution; practically, we do dfs over the
 * edges and pair edges in a tail-recursive fashion. (See said proof and comments below for details)
 *
 * Performance:
 * O(V + E), runs the tests in 0.093s using 12,360KB memory.
 */

#include <vector>
#include <stdio.h>

#pragma comment(linker, "/STACK:56777216")

struct edge {
    int a, b; // The vertices of the edge
    bool v;   // Visited during dfs
};

edge edges[100001]; int e; // The input edges and their count
std::vector<int> E[100001]; // The neighboring edges of an vertex
std::pair<int, int> ans[50000]; int a; // The paired edges

void append(int& x, int y) {
    // Given an unpaired edge (or none), merges it with another one, (or adds)
    if(!x) { // No unpaired edge, make this it
        x = y;
    } else if (y) { // Pair it with the current unpaired edge, nullify the unpaired one
        ans[a++] = { x, y };
        x = 0;
    }
}

int dfs(int u, int p);

void visit(int& x, int p) { // Visits all neighbors of vertex p, initially with unpaired edge x
    while(!E[p].empty()) {
        auto e = E[p].back();
        E[p].pop_back();
        if(!edges[e].v) // If we haven't visited that edge
            append(x, dfs(e, p)); // Dfs over it and pair anything unpaired with x
    }
}

int dfs(int u, int p) {
    // Pairs edges in a graph starting with edge u, arriving from vertex p. An invariant
    // here is that any unpaired edge when the function exits neighbors p
    edges[u].v = true;

    int x = 0; // The unpaired edge
    int p2 = p^edges[u].a^edges[u].b; // Use xor magic to get the opposite vertex

    visit(x, p2); // Visit and merge the opposite edges first
    append(x, u); // Join any unpaired edge with the current edge if we can
    visit(x, p);  // Same with the edges on the parent vertex
    return x; // Return any unpaired edge, used to check for failure in the root call
}

int main() {
    int i, j;

    char line[20];
    while (fgets(line, 20, stdin)) {
        sscanf(line, "%d %d", &i, &j);
        edges[++e] = { std::min(i, j), std::max(i, j) };
        E[i].push_back(e);
        E[j].push_back(e);
    }

    for(int i = 1; i <= e; i++)
        if(!edges[i].v && dfs(i, edges[i].a)) // We were unable to pair some edge
            return printf("-1\n"), 0;

    printf("%d\n", a);
    for(int i = 0; i < a; i++) {
        auto& p = ans[i];
        int a[4] = { edges[p.first].a, edges[p.first].b, edges[p.second].a, edges[p.second].b };
        for(int i = 3; a[1] != a[2]; i++ ) // Make sure the shared vertex is in the middle
            std::swap(a[1], a[i%4]);
        printf("%d %d %d\n", a[0], a[1], a[3]);
    }
}
