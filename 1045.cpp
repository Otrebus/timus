/* 1045. Funny Game - http://acm.timus.ru/problem.aspx?num=1045
 *
 * Strategy:
 * Minimax dfs.
 *
 * Performance:
 * O(n), runs the test cases in 0.015s using 316KB memory.
 */

#include <stdio.h>
#include <vector>
#include <algorithm>

struct node
{
    std::vector<node*> v;
    bool visited;
} nodes[1001];

bool dfs(node* n)
{
    n->visited = true;
    return std::find_if(n->v.begin(), n->v.end(), [] (node* n) { return !n->visited && !dfs(n); })
               != n->v.end();
}

int main()
{
    int n, k, x, y, min = 1001;
    scanf("%d %d", &n, &k);
    while(--n)
        scanf("%d %d", &x, &y), nodes[x].v.push_back(nodes+y), nodes[y].v.push_back(nodes+x);
    nodes[k].visited = true;
    for(auto v : nodes[k].v)
        if(!dfs(v))
            min = std::min(min, v-nodes);
    if(min != 1001)
        printf("First player wins flying to airport %d", min);
    else
        printf("First player loses");
    std::system("pause");
}