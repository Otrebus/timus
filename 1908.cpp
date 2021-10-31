/* 1908. Brute-force Search - http://acm.timus.ru/problem.aspx?num=1908
 *
 * Strategy:
 * Build a trie from the given strings, and for each node associate it with optimal number of clicks
 * to explore that subtree if we arrive at that node either by means of fast-forwarding or by
 * navigating to that node specifically. For each node we can then calculate the optimal branch to
 * take last and fast forward through (avoiding clicks at that node for every ending of that branch).
 *
 * Performance:
 * O(n), runs the tests in 0.015s using 17,792KB memory
 */

#include <stdio.h>
#include <algorithm>

#pragma comment(linker, "/STACK:16777216")

const char nc = 'z'-'a'+1;
char s[100001];
int n;

struct node {
    int v[nc];  // The children, one for each character
    int c;      // The number of endings (leafs) reachable from this node
    int s[2];   // The optimal number of clicks to explore this subtree if arriving through
} nodes[100001];// fastforwarding vs by clicking this node specifically
int m = 1;

int dfs(int n) { // Iterates through the trie and calculates s[2] and returns c
    node& nod = nodes[n];
    int s = 0; // The total cost of the subtree when clicking through all the options
    for(int i = 0; i < nc; i++) {
        int w = nod.v[i];
        node& nw = nodes[w];
        if(w) {
            nod.c += dfs(w);     // Add together the number of endings (leafs) below this node
            s += nw.s[1] + nw.c; // For every leaf of the subtree, we have to click this node once
        }
    }
    if(!nod.c) // This is a leaf
        return nod.c = 1;

    nod.s[0] = nod.s[1] = 1e9;
    for(int i = 0; i < nc; i++) {
        int w = nod.v[i];
        node& nw = nodes[nod.v[i]];
        if(w) {
            // If we pick this branch last, we can fastforward through this node, which means we
            // don't have to click this node once for every ending in the fastforwarded direction
            int ds = s + nw.s[0] - nw.s[1];
            // The cost of exploring the subtree by fastforwarding here, if we ff through the child
            nod.s[0] = std::min(nod.s[0], ds - nw.c);
            // The cost of exploring the subtree by clicking this node, if we ff through the child
            nod.s[1] = std::min(nod.s[1], ds);
        }
    }
    return nod.c;
}

void add(char* s) {
    int node = 0;
    for(int i = 0; s[i]; i++) {
        int& next = nodes[node].v[s[i]-'a'];
        node = next ? next : (next = m++);
    }
}

int main() {
    scanf("%d", &n);

    for(int i = 0; i < n; i++) {
        scanf("%s", s);
        add(s);
    }
    dfs(0);
    printf("%d", nodes[0].s[1]);
}
