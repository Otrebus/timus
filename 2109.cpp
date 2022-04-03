/* 2109. Tourism on Mars - http://acm.timus.ru/problem.aspx?num=2109
 *
 * Strategy:
 * Calculate the least common ancestors (with 1 being the root of the tree) of any adjacent pair
 * of cities i and i+1 using Tarjan's algorithm. Then form a segment tree with those values to
 * quickly query for the minimum such an LCA over a range.
 *
 * Performance:
 * O(qnlog n), runs the tests in 1.562s using 32,044B memory.
 */

#include <vector>
#include <stdio.h>

#pragma comment(linker, "/STACK:16777216")

struct node {
    std::vector<node*> out;
    bool v, c; // Visited, lca finished
    node* parent; // Disjoint-set parent
    int rank; // Disjoint-set rank
    int d; // Depth from root
    node* a;
} nodes[200001];

node* A[200000];
node* T[525000];

int x, y, n, q, r = 1;

void link(node* a, node* b) {
    if(a->rank > b->rank)
        b->parent = a;
    else {
        a->parent = b;
        if(a->rank == b->rank)
            b->rank++;
    }
}

node* find(node* s) { // Disjoint set find
    if(s != s->parent)
        s->parent = find(s->parent);
    return s->parent;
}

void join(node* a, node* b) { // Disjoint set union 
    link(find(a), find(b));
}

void lca(node* u, int d) {
    // Tarjan's LCA algorithm
    u->parent = u;
    u->v = true;
    u->a = u;
    u->d = d;

    for(auto& v : u->out) {
        if(!v->v) {
            lca(v, d+1);
            join(u, v);
            find(u)->a = u;
        }
    }
    u->c = true;
    node* v = u-1;
    if(v->c)
        A[u-1-nodes-1] = find(v)->a;
    v = u+1;
    if(v->c)
        A[u-nodes-1] = find(v)->a;
}

node* build(int i, int L, int R) {
    // Builds the segment tree, recursive edition
    int mid = (L+R)/2;
    if(L < R) {
        node* m1 = build(i*2, L, mid);
        node* m2 = build(i*2+1, mid+1, R);
        T[i] = m1->d < m2->d ? m1 : m2;
    }
    return T[i];
}

void build() {
    // Builds the segment tree
    std::memset(T, 0, sizeof(T));

    while(r < n-1) // Calculate the necessary size of the tree
        r*=2;
    for(int i = 0; i < n-1; i++)
        T[r+i] = A[i]; // Assign the lowest row
    for(int i = n-1; i < r; i++)
        T[r+i] = nodes; // Assign the rest
    nodes[0].d = 1e9;
    build(1, 1, r);
}

node* findMin(int i, int l, int r, int L, int R) {
    // Finds the minimum element of the array between indices l and r, starting the tree search at
    // node i which covers the subrange L to R
    if(l <= L && r >= R)
        return T[i]; // The node covers the entire searched range
    
    int mid = (L+R)/2;
    node* m1 = nodes, *m2 = nodes;
    if(l <= mid) // Search left subtree
        m1 = findMin(i*2, l, r, L, mid);
    if(r > mid) // Search right subtree
        m2 = findMin(i*2+1, l, r, mid+1, R);
    return m1->d < m2->d ? m1 : m2;
}

int main() {
    scanf("%d", &n);
    for(int i = 0; i < n-1; i++) {
        scanf("%d %d", &x, &y);
        nodes[x].out.push_back(&nodes[y]);
        nodes[y].out.push_back(&nodes[x]);
    }
    lca(nodes+1, 0);

    build();
    scanf("%d", &q);
    while(q--) {
        scanf("%d %d", &x, &y);
        printf("%d\n", x == y ? x : findMin(1, std::min(x, y), std::max(x, y)-1, 1, r)-nodes);
    }
}
