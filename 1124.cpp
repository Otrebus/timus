/* 1124. Mosaic - http://acm.timus.ru/problem.aspx?num=1124
 *
 * Strategy:
 * Represent the box and colors as a graph where each box is a node, and each color not in
 * its correct box is an edge points toward its correct box. We are to find the shortest path
 * (Euler path) through the graph.
 *   Since the input is a permutation, this permutation can be expressed as a sequence of
 * transpositions of the input. Each transposition of two nodes in the graph keeps their degrees
 * at 0. Since all nodes then have even degrees, we have an Euler path through each (connected)
 * component of the graph. This means we only need to make extra jumps to each component of the
 * graph besides the one we start in, so the final result is the number of components of the graph
 * above 1, plus the number of edges.
 *
 * Performance:
 * O(MN). This is optimal since the input is of this size. The test cases run in 0.046s time and
 * use 1380 KB of memory.
 */
#include <cstdio>
#include <vector>
#include <stack>

int main()
{
    int M, N, e = 0; // Number of edges
    scanf("%d %d", &M, &N);
    std::vector<bool> vis(M+1, false); // Marks visited nodes when doing dfs
    std::vector<std::vector<int>> adj(M+1); // Adjacency "matrix"

    for(int i = 1; i <= M; i++)
    {
        adj[i].reserve(M);
        for(int j = 1; j <= N; j++)
        {
            int x;
            scanf("%d", &x);
            if(x != i)
            {   // Add an edge for every color not in its own box
                e++;
                adj[i].push_back(x);
            }
        }
    }

    int c = 0; // Number of connected components in the graph
    std::vector<int> stack(M*N);
    int top = 0;
    for(int i = 1; i <= M; i++)
    {   // Simple dfs; count the number of connected components
        if(!vis[i] && !adj[i].empty())
        {
            c++;
            stack[top++] = i;
            while(top != 0)
            {
                int k = stack[--top];
                vis[k] = true;
                for(auto it = adj[k].begin(); it < adj[k].end(); it++)
                {
                    if(!vis[*it])
                        stack[top++] = *it;
                }
            }
        }
    }

    int ans = e + std::max(0, c-1);
    printf("%d\n", ans);
    return 0;
}
