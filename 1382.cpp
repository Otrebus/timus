/* 1382. Game with Cards - http://acm.timus.ru/problem.aspx?num=1382
 *
 * Strategy:
 * For each pair of statements (1, 2) with index i we have four logical variables, namely 1; not 1;
 * 2; not 2, which are inserted at node i, i+1, i+2 and i+3 in an implication graph which we assign
 * edges to with the given rules, and solve using the standard way of solving such graphs: any node
 * is true iff we can't reach its inverse and if any implication doesn't conflict with earlier
 * assignments.
 *
 * Performance:
 * O(N^2), runs the tests in 0.062s using 4,248KB memory.
 */

#include <stdio.h>
#include <vector>

int N;

struct { // The given statements
    int a, b, c;
} A[1000];

struct { // A node in the implication graph
    std::vector<int> v; // Outgoing edges
    bool b, t; // Visited, truth value
} nodes[4000];

bool _dfs(int i, int e) {
    // From node i seeks until it encounters node e (which leads to a contradiction)
    if(i == e || nodes[i^1].t) // If e is reached, or the inverse of this node is true, fail
        return true;
    if(nodes[i].b) // Already visited
        return false;
    nodes[i].b = true;

    for(auto u : nodes[i].v) // For each node, recurse
        if(_dfs(u, e))
            return true;
    return false;
}

bool dfs(int i, int e) {
    // Cleans the 'visited' info and does dfs
    for(int i = 0; i < N*4; i++)
        nodes[i].b = false;
    return _dfs(i, e);
}

void dfs2(int i) {
    // Assigns true to every node implied from i
    if(nodes[i].t)
        return;
    nodes[i].t = true; // Assign this node true
    for(auto u : nodes[i].v) // Recurse for each node
        dfs2(u);
}

void add(int i, int a, int j, int b) {
    nodes[i*4+a].v.push_back(j*4+b);
}

int main() {
    scanf("%d", &N);

    for(int i = 0; i < N; i++)
        scanf("%d %d %d", &A[i].a, &A[i].b, &A[i].c);

    for(int i = 0; i < N; i++) {
        // Add the internal implications of the statement (1 is true means 2 is not true and its
        // contrapositive, and same for 2 -> 1).
        add(i, 0, i, 3), add(i, 1, i, 2), add(i, 2, i, 1), add(i, 3, i, 0);

        for(int j = 0; j < N; j++) {
            if(i != j) {
                if(A[j].a == A[i].a) // Two nodes claim the same card, if we are telling the truth
                    add(i, 0, j, 1); // they must lie
                if(A[j].c == A[i].a) // Someone claims someone has a card we have
                    add(i, 0, j, 3), add(j, 2, i, 1); // If we tell the truth, they lie, and reverse
                if(A[j].c == A[i].c) // Someone claims someone has a card, but so did we
                    add(i, 2, j, 3); 
                if(A[j].b == i+1)    // Someone claims we are lying about our card
                    add(i, 0, j, 3), add(j, 2, i, 1);
            }
        }
    }

    // Solve the implication graph
    for(int i = 0; i < N; i++)
        if(!dfs(i*4, i*4+1))
            dfs2(i*4);

    for(int i = 0; i < N; i++)
        printf("%d ", nodes[i*4].t ? 1 : 2);
}
