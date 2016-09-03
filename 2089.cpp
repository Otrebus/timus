/* 2089. Experienced coach - http://acm.timus.ru/problem.aspx?num=2089
 *
 * Strategy:
 * For each component of the corresponding graph, we need to produce an injective mapping between
 * the edges of the component to the vertices of the component, which is only possible if there are
 * at least as many vertices as edges. For any given component, this is only possible in the case of
 * having n vertices and n-1 edges (a tree), or n vertices and n edges (a tree with one cycle). We
 * thus assume that each component is a tree, depth-first search through the tree and map each edge
 * to one of its child nodes (note that this leaves the root node of the component unassigned). If a
 * cycle was found we first check that this is only the first such cycle, and reverse the mapping
 * for that cycle; specifically, we assign each edge to the parent vertex all the way from where we
 * detected the cycle back to the root.
 *
 * Performance:
 * O(n + k), runs the tests in 0.296s using 8,980KB memory.
 */

#include <iostream>
#include <vector>

#pragma comment(linker, "/STACK:16777216")

const int maxn = 100000;

struct Edge
{
    int ans; // The number of the vertex, or line of input, that this edge is assigned to
    bool visited;
} edges[maxn+1];

struct Node
{
    std::vector<std::pair<Edge*, Node*>> v; // Edges of this vertex
    bool visited, assigned;
    Edge* parentEdge; // Parent edge of the dfs tree
    Node* parentNode;
} nodes[maxn+1];

int nedges = 0;

bool dfs(Node* node)
{
    if(node->visited)
        return true;
    node->visited = true;

    for(auto p : node->v) // For each outgoing edge of this vertex
    {
        auto e = p.first;
        auto n = p.second;
        if(e->visited)
            continue;
        e->visited = true;
        if(n->visited) // We found a cycle, so backtrack and invert the assignments
        {
            for(auto n = node; n; e = n->parentEdge, n = n->parentNode)
            {
                if(!n->parentNode && n->assigned)
                    return false;
                n->assigned = e->ans = n - &nodes[0];
            }
        }
        else // Assign the child to this edge and recurse
        {
            n->parentNode = node;
            n->parentEdge = e;
            n->assigned = e->ans = n - &nodes[0];
            if(!dfs(n))
                return false;
        }
    }
    return true;
}

int main()
{
    std::ios::sync_with_stdio(false);
    int n, k, x, y;
    std::cin >> n >> k;
    for(int i = 1; i <= n; i++)
    {
        std::cin >> x >> y;
        Edge* e = &edges[++nedges];
        nodes[x].v.push_back( { e, &nodes[y] } );
        nodes[y].v.push_back( { e, &nodes[x] } );
    }
    for(int i = 1; i <= k; i++)
    {
        if(!dfs(&nodes[i]))
        {
            std::cout << "No";
            return 0;
        }
    }
    std::cout << "Yes\n";
    for(int i = 1; i <= n; i++)
        std::cout << edges[i].ans << "\n";
}