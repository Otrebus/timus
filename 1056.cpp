/* 1056. Computer Net - http://acm.timus.ru/problem.aspx?num=1056
 *
 * Strategy:
 * Find the longest path (the diameter) in the spanning tree by first finding the farthest node
 * from the current node, and then from that node do the same thing. The requested node(s) are the
 * median node(s) of the path between those two diameter nodes. (If the center nodes were elsewhere,
 * you could make a path longer than half the length of the longest path to those nodes by starting
 * from one of the endpoints of the longest path that we found and travelling to the supposed
 * center nodes, which would imply those center nodes are part of an even longer path, a
 * contradiction.)
 *
 * Performance:
 * O(n), runs the test cases in 0.031s using 912KB memory.
 */

#include <vector>
#include <stdio.h>

struct node
{
    node* p; // Previous node in the second dfs
    std::vector<node*> v; // Neighbors
    short d; // Distance
    char visited; // Marks the node visited when set
} nodes[10001];

int main()
{
    int n;
    scanf("%d", &n);

    for(int i = 2; i <= n; i++)
    {
        int x;
        scanf("%d", &x);
        nodes[x].v.push_back(nodes+i);
        nodes[i].v.push_back(nodes+x);
    }
    node* maxnode; // Farthest node
    short max = 0; // Farthest node distance
    std::vector<node*> s;
    s.reserve(n);
    s.push_back(nodes+1);
    s.back()->visited = 1;
    while(!s.empty()) // dfs
    {
        auto node = s.back();
        s.pop_back();
        if(node->d > max)
            max = node->d, maxnode = node;

        for(auto n : node->v)
        {
            if(!n->visited)
            {
                n->visited = 1;
                s.push_back(n);
                n->d = node->d+1;
            }
        }
    }
    s.clear();
    s.push_back(maxnode);
    maxnode->visited = 2; // They were all marked 1 previously, now we use 2
    maxnode->d = 0;
    max = -1;
    while(!s.empty()) // Same thing as last time ..
    {
        auto node = s.back();
        s.pop_back();
        if(node->d > max)
            max = node->d, maxnode = node;

        for(auto n : node->v)
        {
            if(n->visited != 2)
            {
                n->visited = 2;
                s.push_back(n);
                n->d = node->d+1;
                n->p = node; // .. except now we record the nodes in the path
            }
        }
    }
    std::vector<int> ans;
    for(auto n = maxnode; n; n = n->p) // Go through the parent node and record the medians
        if(maxnode->d/2 == n->d || maxnode->d/2 + maxnode->d%2 == n->d)
            ans.push_back(n - nodes);
    if(ans.size() > 1 && ans[0] > ans[1])
        std::swap(ans[1], ans[0]);
    for(auto a : ans)
        printf("%d ", a);
}