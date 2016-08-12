/* 1463. Happiness to People! - http://acm.timus.ru/problem.aspx?num=1463
 *
 * Strategy:
 * The standard algorithm for calculating the diameter of a spanning tree is used for each component
 * of the graph. The algorithm is: first select a random node z of the component and do a
 * depth-first search to find the node x furthest away from z (that brings maximum happiness when
 * traveled to from z), then do another similar depth-first search from node x to find the node y
 * furthest from x. The optimal path of the component is the path between node x and node y.
 *
 * Performance:
 * O(n), runs the tests in 0.046s using 2,800KB memory (rated 7/517).
 */

#include <stdio.h>
#include <vector>

const int maxn = 50000;

struct Node
{
    std::vector<std::pair<Node*, int>> v; // Neighbors
    int A;
    int visited; // We visit the graph two times, so this is used to keep track of which nodes we've
                 // visited, rather than to have a visit flag that we clear after the first visit
    Node* parent; // Node that called the most recent dfs search on us
} nodes[maxn+1];

// Arguments: current node, value accumulated along this path, maximum node and value so far
void dfs(Node* node, int val, Node*& maxnode, int& maxval)
{
    for(auto p : node->v)
    {
        auto n = p.first;
        auto c = p.second;
        auto val2 = val + c + n->A;
        if(n->visited == node->visited)
            continue;
        n->visited = node->visited;
        n->parent = node;
        if(val2 > maxval)
            maxval = val2, maxnode = n;
        dfs(n, val2, maxnode, maxval);
    }
}

int main()
{
    int n, k, x, y, c, visit = 1;
    scanf("%d %d", &n, &k);
    for(int i = 1; i <= n; i++)
        scanf("%d", &nodes[i].A);
    for(int i = 1; i <= k; i++)
    {
        scanf("%d %d %d", &x, &y, &c);
        nodes[x].v.push_back( { nodes+y, c } );
        nodes[y].v.push_back( { nodes+x, c } );
    }
    Node* maxnode = 0; // The furthest node of the best path found so far
    int maxval = 0;
    for(int i = 1; i <= n; i++)
    {
        // If we haven't visited this component yet, start dfs from here ..
        if(nodes[i].visited)
            continue;
        Node* cmaxnode = nodes+i; // (same as maxnode but for this component only)
        int cmaxval = 0;
        nodes[i].visited = ++visit;
        dfs(nodes+i, 0, cmaxnode, cmaxval);
        // .. and then start another dfs from the node that turned out to be furthest away
        cmaxnode->parent = 0;
        cmaxval = cmaxnode->A;
        cmaxnode->visited = ++visit;
        dfs(cmaxnode, cmaxnode->A, cmaxnode, cmaxval);
        if(cmaxval > maxval)
            maxnode = cmaxnode, maxval = cmaxval;
    }
    // Fill in the output using the parent information along the best path
    std::vector<Node*> list;
    for(auto n = maxnode; n; n = n->parent)
        list.push_back(n);
    printf("%d\n%d\n", maxval, list.size());
    for(auto it = list.rbegin(); it < list.rend(); it++)
        printf("%d ", *it-nodes);
}