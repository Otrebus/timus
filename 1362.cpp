/* 1362. Classmates 2 - http://acm.timus.ru/problem.aspx?num=1362
 *
 * Strategy:
 * Greedy; use dfs from Tanya's node to recursively calculate the optimal time for each of her
 * colleagues, and sort these nodes according to time taken. The optimal time for Tanya is the
 * time taken for the message to spread when Tanya calls the colleagues (nodes) by order of
 * decreasing time. This is done recursively.
 *
 * Performance:
 * O(n log n), runs the test cases in 0.078s using 5296KB memory.
 */

#include <stdio.h>
#include <vector>
#include <stack>
#include <algorithm>

const int maxn = 100000;

struct Node
{
    std::vector<Node*> v;
    Node* parent; // Parent node of this node using Tanya as root
    int t; // Optimal time
} nodes[maxn+1];

int main()
{
    int N, T, x;
    scanf("%d", &N);
    for(int i = 1; i <= N; i++)
    {
        while(true)
        {
            scanf("%d", &x);
            if(!x)
                break;
            nodes[i].v.push_back(&nodes[x]);
            nodes[x].v.push_back(&nodes[i]);
        }
    }
    scanf("%d", &T);

    // Dfs iteratively through the nodes
    std::stack<std::pair<Node*, bool>> s;
    s.push( { &nodes[T], true } ); // The second argument is used to indicate that all subnodes
    while(!s.empty())              // have been called and we are unwinding our dfs call
    {
        auto p = s.top();
        s.pop();
        Node* node = p.first;
        bool forward = p.second;
        if(forward) // We arrived here from our parent
        {
            s.push( { node, false } ); // Push false first (see the else {})
            for(auto n : node->v)
                if(n != node->parent)
                    n->parent = node, s.push( { n, true } ); // Push all actual nodes
        }
        else // { node, false } on the stack means node finished all subcalls
        {
            // Order the descendant nodes by reverse size
            std::sort(node->v.begin(), node->v.end(), [](Node* a, Node* b) { return a->t > b->t; });
            int max = 0, i = 1; // i is the time elapsed since we received our call
            for(auto n : node->v)
                if(n != node->parent)
                    max = std::max(i++ + n->t, max);
            node->t = max;
        }
    }
    printf("%d", nodes[T].t);
}