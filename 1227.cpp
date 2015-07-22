/* 1227. Rally Championship - http://acm.timus.ru/problem.aspx?num=1227
 *
 * Strategy:
 * Assume the graph is a forest of trees, and do a depth first search (dfs) starting at every node
 * which calculates for each node a) the longest path overall of this subtree, and b) the longest
 * path going from any child up to this node. This information is recursively calculated througout
 * the dfs. If the graph contains a cycle, we notice this during the dfs and output in the 
 * affirmative (since this constitutes an infinite path).
 * 
 * Performance:
 * O(M), which is optimal since the input is of this size. The test cases run in 0.015s time and
 * use 256 KB of memory.
 */

#include <stdio.h>
#include <vector>
#include <algorithm>

struct node
{
    std::vector<std::pair<node*, int>> out; // Outgoing vertices, with weights
    bool visited;
    node() : visited(false) {}
};

// Returns the pair as (the longest path to any descendant, the longest path of the entire subtree)
std::pair<int, int> dfs(node* u, node* parent)
{   // max, max2 = the two longest paths from this node down to any descendant
    // maxtot = the longest path of the entire subtree
    int max = 0, max2 = 0, maxtot = 0;
    for(auto it = u->out.begin(); it < u->out.end(); it++)
    {
        if(it->first == parent) // Don't go back the way we came
            continue;
        if(it->first->visited)  // Loop found!
            return std::make_pair(1000000, 1000000);
        it->first->visited = true;
        auto p = dfs(it->first, u);
        int t = p.first + it->second; // Longest straight length of track plus edge length
        maxtot = std::max(p.second, maxtot); // Longest total length of the entire subtree
        if(t > max)
        {
            max2 = max;
            max = t;
        }
        else if(t > max2)
            max2 = t;
    }
    // We might make a new longest path of the subtree by combining the two longest straight paths
    return std::make_pair(max, std::max(max+max2, maxtot));
}

int main()
{
    int M, N, S;
    scanf("%d %d %d", &M, &N, &S);

    if(N >= M) // Guarantees a loop
    {
        printf("YES\n");
        return 0;
    }

    std::vector<node> nodes(M+1);
    while(N--)
    {
        int P, Q, R;
        scanf("%d %d %d", &P, &Q, &R);
        nodes[P].out.push_back(std::make_pair(&nodes[Q], R));
        nodes[Q].out.push_back(std::make_pair(&nodes[P], R));
    }
    for(int i = 1; i <= M; i++)
    {
        if(!nodes[i].visited)
        {
            auto p = dfs(&nodes[i], 0);
            if(p.second >= S)
            {
                printf("YES\n");
                return 0;
            }
        }
    }
    printf("NO\n");
    return 0;
}