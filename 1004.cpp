/* 1004. Sightseeing Trip - http://acm.timus.ru/problem.aspx?num=1004
 *
 * Strategy:
 * For each node n: run Dijkstra to get the shortest path tree, then add a single (existing) edge to
 * every possible pair of that tree to find the shortest Y-shaped loop starting at n. Report the 
 * shortest such loop after having iterated over all starting nodes. Any such loop will have a
 * degenerate "stem" of the Y and hence be a true loop.
 *
 * Performance:
 * O(N^3 log N), runs the test cases in 0.046s using 424KB memory.
 */

#include <stdio.h>
#include <limits>
#include <algorithm>
#include <queue>
#include <functional>

struct Node
{
    int d; // Shortest distance here
    bool visited;
    std::vector<Node*> v; // Neighbors
    Node* parent; // Parent in the shortest path tree
} nodes[101];

int A[101][101]; // Adjacency matrix

const int inf = std::numeric_limits<int>::max()/3;
int min = inf; // Length of the shortest loop so far
std::vector<int> minv; // The actual nodes of that loop

// Returns the id of this node and its every ancestor except the original ancestor
std::vector<int> getNodes(Node* start, Node* node)
{
    std::vector<int> ret;
    while(node != start)
    {
        ret.push_back(node-nodes);
        node = node->parent;
    }
    return ret;
}

// Find the shortest Y-shaped loop in the shortest path tree that starts at N
void search(int s, int N)
{
    using pin = std::pair<int, Node*>;
    std::priority_queue<pin, std::vector<pin>, std::greater<pin>> q;
 
    for(int i = 1; i <= N; i++)
        nodes[i].d = inf, nodes[i].visited = false;
    nodes[s].d = 0;

    // Dijkstra
    q.push( { 0, nodes+s } );
    while(!q.empty())
    {
        auto p = q.top();
        q.pop();
        auto node = p.second;
        if(node->visited)
            continue;
        node->visited = true;
        for(auto it = node->v.begin(); it < node->v.end(); it++)
        {
            auto n = *it;
            int i = node - nodes, j = n - nodes;
            if(!n->visited && n->d > node->d + A[i][j])
            {
                n->parent = node;
                n->d = node->d + A[i][j];
                q.push( { n->d, n });
            }
        }
    }

    // Go through every pair of the shortest path tree and try closing loops
    std::pair<int, int> minpair;
    auto mincost = inf;
    for(int i = 1; i <= N; i++)
        for(int j = 1; j <= N; j++)
            if(i != j && nodes[i].parent != &nodes[j] && nodes[j].parent != &nodes[i])
                if(mincost > nodes[i].d + nodes[j].d + A[i][j])
                    mincost = nodes[i].d + nodes[j].d + A[i][j], minpair = { i, j };
    // A shortest new loop was found
    if(min > mincost)
    {
        min = mincost;
        minv.clear();
        minv.push_back(s); // Push starting node
        auto f = getNodes(nodes+s, nodes+minpair.first); // From starting node to one prong
        auto g = getNodes(nodes+s, nodes+minpair.second); // To the other prong of the Y
        minv.insert(minv.begin(), f.begin(), f.end());
        std::reverse(g.begin(), g.end()); // Reverse one of the above so we get the loop
        minv.insert(minv.begin(), g.begin(), g.end());
    }
}

int main()
{
    while(true)
    {
        int N, M;
        scanf("%d", &N);
        if(N == -1)
            break;
        scanf("%d", &M);
        for(int i = 0; i < 101; i++)
            for(int j = 0; j < 101; j++)
                A[i][j] = inf;
        while(M--)
        {
            int a, b, l;
            scanf("%d %d %d", &a, &b, &l);
            if(l < A[a][b])
                A[a][b] = A[b][a] = l;
        }

        for(int i = 1; i <= N; i++)
            for(int j = 1; j < i; j++)
                if(A[i][j] != inf)
                    nodes[i].v.push_back(nodes+j), nodes[j].v.push_back(nodes+i);
        min = inf;
        for(int i = 1; i <= N; i++) // Dijkstra from every node
            search(i, N);
        if(min == inf)
            printf("No solution.");
        else
            for(auto i : minv)
                printf("%d ", i);
        printf("\n");
        for(int i = 0; i < 101; i++)
            nodes[i] = { 0, false, {}, 0 };
    }
}