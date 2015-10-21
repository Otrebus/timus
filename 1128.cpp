/* 1128. Partition into Groups - http://acm.timus.ru/problem.aspx?num=1128
 *
 * Strategy:
 * Start with all children in the first group, then for any child in each group, move a child
 * with more than one adversary to the other group.
 *
 * Performance:
 * O(n) since we remove one edge for each movement; the solution runs in 0.031s using 1052KB
 * memory, which is the second fastest solution for the compiler used.
 */

#include <stdio.h>
#include <vector>
#include <unordered_set>

struct node
{
    std::vector<node*> adj;
    int parity, nAdj;
};

const int maxn = 7164;

std::unordered_set<node*> v[4]; // Maps each index i to the set of all nodes with i neighbors
int size[2]; // The size of the two groups we assign each node to
node nodes[maxn];

int main()
{
    int N;
    scanf("%d", &N);
    size[0] = N;
    for(int i = 0; i < 4; i++)
        v[i].reserve(maxn);
    for(int i = 1; i <= N; i++)
    {
        int k;
        scanf("%d", &k);
        v[k].insert(nodes+i);
        nodes[i].nAdj = k;
        for(int l = 0; l < k; l++)
        {
            int j;
            scanf("%d", &j);
            if(i < j)
            {
                nodes[j].adj.push_back(nodes+i);
                nodes[i].adj.push_back(nodes+j);
            }
        }
    }

    for(int k = 3; k > 1; k--)
    {
        if(v[k].empty())
            continue;
        auto it = v[k].begin();
        auto n = *it;
        v[k].erase(it);
        // Rip out this node from this group, update the info of its neighbors
        for(auto a : n->adj)
        {
            if(a->parity == n->parity)
            {
                v[a->nAdj].erase(a);
                a->nAdj--;
                v[a->nAdj].insert(a);
                n->nAdj--;
            }
        }
        size[n->parity]--;
        n->parity = (n->parity + 1) % 2;
        size[n->parity]++;
        // Insert this node to the other group, update the info of its neighbors
        for(auto a : n->adj)
        {
            if(a->parity == n->parity)
            {
                v[a->nAdj].erase(a);
                a->nAdj++;
                v[a->nAdj].insert(a);
                n->nAdj++;
            }
        }
        v[n->nAdj].insert(n);
        k = 4;
    }

    int parity = size[0] == size[1] ? nodes[1].parity : size[0] > size[1] ? 1 : 0;
    printf("%d\n", size[parity]);
    for(int i = 1; i <= N; i++)
        if(nodes[i].parity == parity)
            printf("%d ", i);
    return 0;
}