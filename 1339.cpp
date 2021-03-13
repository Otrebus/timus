/* 1339. Babies - http://acm.timus.ru/problem.aspx?num=1339
 *
 * Strategy:
 * For any non-cycle, start with the first edge into the path and assign every odd edge as a pair.
 * Then assign every other edge in the remaining components (cycles) as pairs, and finally assign
 * unconnected pairs of nodes arbitrarily.
 *
 * Performance:
 * O(k), runs the tests in 0.078s using 11,848KB memory.
 */

#include <stdio.h>

struct node {
    node* n, *p, *a; // Next, previous, answer (assigned node)
} nodes[500001];
int k, x;

int main() {
    scanf("%d", &k);

    for(int i = 1; i <= k; i++) {
        scanf("%d", &x);
        if(x)
            nodes[i].n = nodes+k+x, nodes[k+x].p = nodes+i;
    }

    for(int i = 1; i <= k; i++) {
        scanf("%d", &x);
        if(x)
            nodes[k+i].n = nodes+x, nodes[x].p = nodes+k+i;
    }

    for(int i = 1; i <= k*2; i++) // For every non-cycle path, pair up every other edge
        if(!nodes[i].p)
            for(node* n = nodes+i; n && n->n; n = n->n->n) 
                n->a = n->n, n->n->a = n;

    for(int i = 1; i <= k; i++) // For everything remaining (cycles), do the same
        for(node* n = nodes+i; n->n && !n->a; n = n->n->n)
            n->a = n->n, n->n->a = n;

    int j = 1 + k;
    for(int i = 1; i <= k; i++) {
        node* a = nodes[i].a;
        if(!a) { // Still unassigned, find some unconnected node
            while(nodes[j].a)
                j++;
            a = nodes+j++;
        }
        printf("%d ", a-k-nodes);
    }
}


