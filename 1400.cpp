/* 1400. Cellular Characters - http://acm.timus.ru/problem.aspx?num=1400
 *
 * Strategy:
 * Straightforward; build the graph for the given keyboard, use breadth-first search from every
 * possible starting location, and output the location that gives the lowest sum distance.
 *
 * Performance:
 * O(N^2), runs the test cases in 0.015s using 296KB memory. Rated 20/741.
 */

#include <stdio.h>
#include <vector>
#include <queue>
#include <limits>

struct Node
{
    std::vector<Node*> v;
    int depth;
} nodes[256];

int N, M;

int bfs(Node* start)
{
    int ret = 0; // Distance sum
    for(auto& n : nodes)
        n.depth = std::numeric_limits<int>::max();
    std::queue<Node*> Q;
    start->depth = 0;
    Q.push(start);
    while(!Q.empty())
    {
        auto node = Q.front();
        ret += node->depth;
        Q.pop();
        for(auto n : node->v)
            if(n->depth > node->depth+1)
                n->depth = node->depth+1, Q.push(n);
    }
    return ret;
}

int getNode(int i, int j) { return i*M+j; } // Nodes are counted top left to bottom right
void addConnection(Node* a, Node* b) { a->v.push_back(b), b->v.push_back(a); }

void buildGraph()
{
    int height = N/M + bool(N%M);
    for(int i = 0; i < height; i++)
        for(int j = 0; j < M-1 && getNode(i+1, j) < N; j++)
            addConnection(nodes+getNode(i,j),nodes+getNode(i+1,j));
    for(int i = 0; i < N; i++)
        addConnection(nodes+i, nodes+(i+1)%N);
}

int main()
{
    scanf("%d %d", &N, &M);
    buildGraph();
    int mind = std::numeric_limits<int>::max(), mini;
    for(int i = 0; i < N; i++) // Find the starting location that gives the shortest sum
    {
        auto d = bfs(nodes+i);
        if(mind > d)
            mind = d, mini = i;
    }
    bfs(nodes+mini); // We fill in the depth values for the shortest depth sum again
    printf("Mean = %.2lf\n", double(mind)/N);
    for(int i = 0; i < N; i++)
    {
        printf("%d ", nodes[i].depth);
        if((i+1) % M == 0)
            printf("\n");
    }
}