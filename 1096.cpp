/* 1096. Get the Right Route Plate! - http://acm.timus.ru/problem.aspx?num=1096
 *
 * Strategy:
 * Let every bus be represented by a node in a graph and every possible exchange between buses an
 * edge. We can then simulate exchanges of plates by travelling through this graph; whenever we go
 * from node A to node B, it means we have the plates of bus A and now switch to the plates bus B,
 * and since we have the plates of bus B, we can also switch plates to whoever bus B could
 * switch plates with. It never makes sense to backtrack and revisit a node, since we at some point
 * already had the plates of that node, and revisiting would never be part of an optimal route.
 * Hence, we can simply do a depth-first search through the graph, starting with the node
 * representing our bus.
 *
 * Performance:
 * O(N), runs the test cases in 0.015s using 464KB memory.
 */

#include <vector>
#include <iostream>
#include <queue>

const int maxn = 1000;

struct Node
{
    int d, x, y; // Optimal depth, our route, backside route
    Node* p; // Parent node of the optimal path
    std::vector<Node*> v; // Neighbors (buses we can exchange plates with)
} nodes[maxn+2];

int main()
{    
    int N, r, x, y;
    std::cin >> N;
    std::vector<std::pair<int, int>> input(N+1);
    std::vector<std::vector<Node*>> routes(2001); // Buses of each route

    for(int i = 1; i <= N; i++)
    {
        std::cin >> input[i].first >> input[i].second;
        nodes[i].x = input[i].first;
        nodes[i].y = input[i].second;
        routes[input[i].first].push_back(&nodes[i]);
        nodes[i].d = maxn;
    }
    
    // Add buses and the plate exchange connections between them
    for(int i = 1; i <= N; i++)
    {
        for(auto& r : routes[input[i].second])
            nodes[i].v.push_back(r);
        for(auto& r : routes[input[i].first])
            if(r != &nodes[i])
                nodes[i].v.push_back(r);
    }

    // Add our bus to the graph
    std::cin >> r >> x >> y;
    for(auto& r : routes[x])
        nodes[N+1].v.push_back(r);
    for(auto& r : routes[y])
        nodes[N+1].v.push_back(r);

    // BFS
    std::queue<Node*> q;
    q.push(&nodes[N+1]);
    nodes[N+1].d = 0;
    while(!q.empty())
    {
        auto node = q.front();
        q.pop();

        // We found our destination, output the result
        if(node->x == r || node->y == r)
        {
            std::vector<Node*> output;
            for(Node* c = node; c != &nodes[N+1]; c = c->p)
                output.push_back(c);
            std::cout << output.size() << "\n";
            for(auto it = output.rbegin(); it < output.rend(); it++)
                std::cout << (*it-nodes) << "\n";
            return 0;
        }
        // Push neighbors to the BFS queue
        for(auto n : node->v)
        {
            if(n->d > node->d + 1)
            {
                n->d = node->d + 1;
                n->p = node;
                q.push(n);
            }
        }
    }
    std::cout << "IMPOSSIBLE";
}