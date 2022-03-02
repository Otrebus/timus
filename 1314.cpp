/* 1314. Chase in Subway - http://acm.timus.ru/problem.aspx?num=1314
 *
 * Strategy:
 * Do a BFS from the first tracked location of the thief to all locations, recording the depth of
 * each node from the starting position. Then, do another search from the last tracked location of
 * the thief to all nodes where the earlier recorded depths increase by one at each jump. The nodes
 * traversed by the last search constitute the answer.
 *
 * Performance:
 * O(NK + Nlog N); runs the test suite in 0.015 using 1,444KB memory.
 */

#include <vector>
#include <stdio.h>
#include <set>
#include <queue>

const int maxn = 32767;

struct Node {
    std::vector<Node*> out; // Edges
    int d; // Depth
    bool v; // Visited
} nodes[maxn+1];

int main()
{
    for(int i = 0; i < maxn+1; i++)
        nodes[i].d = maxn;
    int N, K, M, first, last, id, lastid;

    scanf("%d", &N);
    while(N--) {
        scanf("%d", &K);
        std::vector<int> ids;
        while(K--)
            scanf("%d", &id), ids.push_back(id);
        for(int i = 0; i < ids.size()-1; i++) {
            nodes[ids[i]].out.push_back(nodes+ids[i+1]);
            nodes[ids[i+1]].out.push_back(nodes+ids[i]);
        }
    }

    scanf("%d %d", &M, &first);
    last = first;
    while(--M)
        scanf("%d", &last);

    std::queue<Node*> Q;
    Q.push(nodes+first);
    nodes[first].d = 0;
    while(!Q.empty()) { // First search from the starting point, records depths
        auto node = Q.front();
        Q.pop();
        for(auto n : node->out)
            if(n->d > node->d+1)
                n->d = node->d+1, Q.push(n);
    }

    std::set<Node*> ans;
    Q.push(nodes+last);
    while(!Q.empty()) { // Last search, traverse nodes with unit increase of depth
        auto node = Q.front();
        Q.pop();
        ans.insert(node);
        for(auto n : node->out)
            if(n->d == node->d+1 && !n->v)
                Q.push(n), n->v = true;
    }
    for(auto a : ans)
        printf("%d\n", a-nodes);
}
