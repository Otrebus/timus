/* 1371. Cargo Agency - http://acm.timus.ru/problem.aspx?num=1371
 *
 * Strategy:
 * Depth-first search. If we compute the number of paths going through each node, and also
 * the total length of those paths, we can construct the solution as we search through the
 * tree. See code for more comments.
 *
 * Performance:
 * O(N), runs the test cases in 0.046s using 6,944KB memory.
 */

#pragma comment(linker, "/STACK:16777216")

#include <vector>
#include <stdio.h>

struct Node
{
    long long d; // Total length of the routes entering this node except through the parent
    long long N; // Number of routes entering this node except through the parent 
    std::vector<std::pair<long long, Node*>> v; // Neighbor edges
} nodes[50001];

long long dfs(Node* node, Node* parent)
{
    node->N = 1;
    long long res = 0;
    // First, recurse for the result and for calculating the total number of routes going through
    // this node
    for(auto p : node->v)
    {
        auto n = p.second;
        if(n == parent)
            continue;
        res += dfs(n, node);
        node->N += n->N;
    }
    for(auto p : node->v)
    {
        auto n = p.second;
        auto w = p.first;
        if(n == parent)
            continue;
        // We extend the paths coming through this child node by the edge leading into our node
        node->d += n->d + w*n->N;
        // For the total result, every route entering through this edge also continues through all
        // other edges, so if we for example have one edge with 2 paths, and other edges with a 
        // total of 5 paths, those paths will be connected in 2*5 different ways. 
        res += (n->d + w*n->N)*(node->N-n->N);
    }
    return res;
}

int main()
{
    long long N;
    int a, b, c;
    scanf("%lld", &N);

    for(int i = 0; i < N-1; i++)
    {
        scanf("%d %d %d", &a, &b, &c);
        nodes[a].v.push_back({c,&nodes[b]});
        nodes[b].v.push_back({c,&nodes[a]});
    }
    auto res = dfs(&nodes[1], 0);
    printf("%.4lf", double(2*res)/(N*(N-1)));
}