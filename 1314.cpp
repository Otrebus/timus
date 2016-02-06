/* 1314. Chase in Subway - http://acm.timus.ru/problem.aspx?num=1314
 *
 * Strategy:
 * Do a BFS from the first tracked location of the thief to all locations, recording the depth of
 * each node from the starting position. Then, do another search from the last tracked location of
 * the thief to all nodes where the earlier recorded depths increase by one at each jump. The nodes
 * traversed by the last search constitute the answer.
 *
 * Performance:
 * O(NK log NK); runs the test suite in 0.015 using 968KB memory, rated 33/730.
 */

#include <vector>
#include <stdio.h>
#include <set>
#include <queue>

const int maxn = 32767;

struct Node
{
    std::vector<Node*> v;
    int depth;
} nodes[maxn+1];

int main()
{
    for(int i = 0; i < maxn+1; i++)
        nodes[i].depth = maxn;
    int N, K, M, first, last, id, lastid;
    scanf("%d", &N);
    while(N--)
    {
        scanf("%d %d", &K, &lastid);
        while(--K)
        {
            scanf("%d", &id);
            int p1 = std::min(id, lastid);
            int p2 = std::max(id, lastid);
            nodes[p1].v.push_back(nodes+p2);
            nodes[p2].v.push_back(nodes+p1);
            lastid = id;
        }
    }
    scanf("%d %d", &M, &first);
    last = first;
    while(--M)
        scanf("%d", &last);
    std::queue<Node*> Q;
    Q.push(nodes+first);
    nodes[first].depth = 0;
    while(!Q.empty()) // First search from the starting point, records depths
    {
        auto node = Q.front();
        Q.pop();
        for(auto n : node->v)
            if(n->depth > node->depth+1)
                n->depth = node->depth+1, Q.push(n);
    }
    std::set<Node*> ans;
    Q.push(nodes+last);
    while(!Q.empty()) // Last search, traverse nodes with unit increase of depth
    {
        auto node = Q.front();
        Q.pop();
        ans.insert(node);
        for(auto n : node->v)
            if(n->depth == node->depth+1)
                Q.push(n);
    }
    for(auto a : ans)
        printf("%d\n", a-nodes);
}