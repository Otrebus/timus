/* 1450. Russian Pipelines - http://acm.timus.ru/problem.aspx?num=1450
 *
 * Strategy:
 * Topological sorting together with relaxation at each considered edge.
 *
 * Performance:
 * O(N + M), running the test cases in 0.062s using 2432KB memory, ranked 15/1510.
 */

#include <stdio.h>
#include <vector>
#include <queue>

const int inf = -500*10000;

struct node
{
    std::vector<std::pair<int, int>> e; // Edges out of this node
    int d; // Maximum distance to this node so far
    int in; // Indegree

    node() : in(0), d(inf) {}
};

node nodes[501];

int main()
{
    int N, M, S, F;
    scanf("%d %d", &N, &M);
    std::queue<node*> q; // Set of nodes with no unvisited ancestors

    for(int i = 0; i < M; i++)
    {
        int x, y, c;
        scanf("%d %d %d", &x, &y, &c);
        nodes[x].e.push_back(std::make_pair(y, c));
        nodes[y].in++;
    }
    scanf("%d %d", &S, &F);
    nodes[S].d = 0;

    for(int i = 1; i <= N; i++)
        if(nodes[i].in == 0)
            q.push(nodes + i); // Initialize q with all nodes that have no ingoing edges
    while(!q.empty())
    {
        auto nod = q.front();
        q.pop();
        for(auto it = nod->e.begin(); it < nod->e.end(); it++)
        {   
            nodes[it->first].d = std::max(nodes[it->first].d, nod->d + it->second); // Relaxation
            nodes[it->first].in--;
            if(nodes[it->first].in == 0) // Node had its last unvisited ancestor visited
                q.push(nodes + it->first);
        }
    }
    if(nodes[F].d >= 0) // If less than zero, the last node can't have S as an ancestor
        printf("%d\n", nodes[F].d);
    else
        printf("No solution\n");
    return 0;
}