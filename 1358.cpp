/* 1358. Cables - http://acm.timus.ru/problem.aspx?num=1358
 *
 * Strategy:
 * The given network is a spanning tree so we can readily assign some node to be the root point
 * at the lower left corner of the allowed coordinate space and then grow the network up and to
 * the right by allocating to each child and their descendants horizontal space proportional to
 * its total number of descendants.
 * 
 * Performance:
 * O(N), runs the test suite in 0.015s using 336KB memory.
 */

#include <iostream>
#include <stdio.h>
#include <vector>
#include <algorithm>

struct Node
{
    int x, y;  // Coordinate assigned
    int depth; // Actually the number of descendants of this node including itself
    std::vector<Node*> v; // Neighbors
} nodes[1001];

int dfs(Node* node, Node* parent)
{
    node->depth = 1;
    int t = node->x;
    for(auto n : node->v)
    {
        if(n != parent)
        {
            n->y = node->y + 2;
            n->x = t;
            node->depth += dfs(n, node);
            t += n->depth*2;
        }
    }
    return node->depth;
}

int main()
{
    int N, x, y;
    std::cin >> N;
    for(int i = 0; i < N-1; i++)
    {
        std::cin >> x >> y;
        nodes[x].v.push_back(nodes+y);
        nodes[y].v.push_back(nodes+x);
    }
    nodes[1].x = -1000;
    nodes[1].y = -1000;
    dfs(nodes+1, 0);
    for(int i = 1; i <= N; i++)
        std::cout << nodes[i].x << " " << nodes[i].y << "\n";
}