/* 1701. Ostap and Partners - http://acm.timus.ru/problem.aspx?num=1701
 *
 * Strategy:
 * Let each workman be represented by an element in a disjoint-set forest; every new input
 * constraint corresponds to a set union operation. Let each element of a set keep track of its
 * wage difference with its parent, and also the difference to the minimum element of its
 * descendants (including itself). This allows us to perform the required consistency checks
 * every time an union operation is performed.
 *
 * Performance:
 * O(n), runs the test cases in 0.046s using 1,156KB memory (rated 4/442).
 */

#include <algorithm>
#include <iostream>

struct Node
{
    Node* parent;
    int rank, dif, difmin;
    bool vassily; // Is Vassily part of this set?
} nodes[50000];

Node* find(Node* s)
{   // Normal path compression stuff, but ..
    if(s != s->parent)
    {   // .. since we update the parent we need to update the wage difference with the parent
        auto tmp = find(s->parent);
        s->dif += s->parent->dif;
        s->parent = tmp;
    }
    return s->parent;
}

bool link(Node* a, Node* b, int dif)
{   // Normal union by rank stuff - depending on who ends up as the root, dif might become inverted
    if(a->rank > b->rank)
        std::swap(a, b), dif = -dif;
    a->parent = b;
    if(a->rank == b->rank)
        b->rank++;
    a->dif = dif;
    // Update the difference to the smallest element of the reformed tree of b
    b->difmin = std::max( b->difmin, a->difmin+dif );
    b->vassily |= a->vassily;
    if(b->vassily) // Need to check that no wage is lower than Vassily's
    {
        find(nodes); // Path from Vassily to his representative element might not be compressed yet
        if(nodes->dif - nodes->parent->difmin < 0)
            return false;
    }
    return true;
}
 
bool join(Node* a, Node* b, int dif)
{
    auto ap = find(a);
    auto bp = find(b);
    if(ap == bp && b->dif + dif != a->dif) // We link within the same set; do consistency check
        return false;
    return link(ap, bp, b->dif + dif - a->dif); // Link the roots together with the given difference
}

int main()
{
    int n, m, d, a, b;
    scanf("%d %d", &n, &m);
    for(int i = 0; i < n; i++)
        nodes[i].parent = &nodes[i];
    nodes[0].vassily = true;
    for(int i = 0; i < m; i++)
    {
        scanf("%d %d %d", &a, &b, &d);
        if(!join(nodes+b, nodes+a, d))
        {
            printf("Impossible after %d statements\n", i+1);
            return 0;
        }
    }
    printf("Possible\n");
    for(int i = 0; i < n; i++)
    {   // Make the minimum element always have wage 0
        int min = find(nodes+i)->difmin;
        printf("%d\n", min-nodes[i].dif);
    }
}