/* 1389. Roadworks - http://acm.timus.ru/problem.aspx?num=1389
 *
 * Strategy:
 * Dynamic programming. For each node we save the optimal result when the parent edge is included in
 * the solution (assigned) and the same for the excluded case. We do a single depth-first search
 * pass through the tree and calculate the best assignment for each node individually while memoizing
 * the above results.
 *
 * Performance:
 * O(N), runs the tests in 0.078s using 9,608KB memory.
 */

#include <stdio.h>
#include <vector>

#pragma comment(linker, "/STACK:40000000")

const int maxn = 100000;

struct Node
{
    std::vector<Node*> v;
    Node* ansNode; // The optimal edge from the parent in the case of an assigned edge
    int ans[2]; // Optimal result for unassigned and assigned
    Node() { ans[0] = ans[1] = -1; }
} nodes[maxn+1];

void dfs(bool assigned, Node* parent, Node* node)
{
    if(node->ans[assigned] != -1) // Already memoized
        return;
    int sum = 0;
    // Assume parent is assigned; consider only unassigned results from children
    for(auto n : node->v)
    {
        if(n == parent)
            continue;
        dfs(false, node, n);
        sum += n->ans[false];
    }
    node->ans[true] = sum;
    if(assigned)
        return;
    node->ans[false] = sum;
    // Assume parent is unassigned; try assigning one child at a time and take the maximum result
    for(auto n : node->v)
    {
        if(n == parent)
            continue;
        dfs(true, node, n);
        int c = sum + n->ans[true] - n->ans[false] + 1;
        if(c > node->ans[false])
        {
            node->ans[false] = sum + n->ans[true] - n->ans[false] + 1;
            node->ansNode = n;
        }
    }
}

// Outputs the optimal result of this subtree to v with parent edge assignment as given
void print(bool assigned, Node* parent, Node* node, std::vector<std::pair<int, int>>& v)
{
    if(assigned)
    {
        for(auto n : node->v)
            if(n != parent)
                print(false, node, n, v);
    }
    else
    {
        if(node->ansNode)
            v.push_back( { node->ansNode-nodes, node-nodes } );
        for(auto n : node->v)
            if(n != parent)
                print(node->ansNode == n, node, n, v);
    }
}

int main()
{
    int N, M;
    scanf("%d %d", &N, &M);
    for(int i = 0; i < M; i++)
    {
        int x, y;
        scanf("%d %d", &x, &y);
        nodes[x].v.push_back(nodes + y);
        nodes[y].v.push_back(nodes + x);
    }
    dfs(false, 0, nodes + 1);
    std::vector<std::pair<int, int>> v;
    print(false, 0, nodes + 1, v);
    printf("%d\n", v.size());
    for(auto p : v)
        printf("%d %d\n", p.first, p.second);
}