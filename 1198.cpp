/* 1198. Jobbery - http://acm.timus.ru/problem.aspx?num=1198
 *
 * Strategy:
 * Run Tarjan's algorithm to find the strongly connected components of the graph, and then pick a
 * root component (with no incoming edges from any other component) and see if that component can
 * reach every other component. If so, all the vertices of that root component is the answer. If
 * not, that component can be reached by no other component either so the answer is the empty set.
 *
 * Performance:
 * O(|V|+|E|), runs in 0.78s using 19,712KB memory.
 */

#include <algorithm>
#include <stdio.h>
#include <stack>
#include <vector>

const int maxn = 2000;

struct Node
{
    int index, low, component;
    bool onStack;
    std::vector<Node*> v;
} nodes[maxn+1];

std::stack<Node*> s;
int in[2001], index = 1, component = 1;

void connect(Node* node) // The recursive part of Tarjan's algorithm
{
    node->low = node->index = index++;
    node->onStack = true;
    s.push(node);
    for(auto n : node->v)
    {
        if(!n->index)
            connect(n);
        if(n->onStack)
            node->low = std::min(node->low, n->low);
    }

    if(node->low == node->index)
    {
        Node* n;
        do
        {
            n = s.top();
            s.pop();
            n->component = component;
            n->onStack = false;
        } while(n != node);
        component++;
    }
}

int dfs(Node* node) // Returns how many nodes this node can reach
{
    int size = node->index = 1;
    for(auto n : node->v)
        if(!n->index)
            size += dfs(n);
    return size;
}

int main()
{
    int N, j;
    scanf("%d", &N);
    for(int i = 1; i <= N; i++)
    {
        while(true)
        {
            scanf("%d", &j);
            if(j == 0)
                break;
            nodes[i].v.push_back(&nodes[j]);
        }
    }
    for(int i = 1; i <= N; i++)
        if(!nodes[i].index)
            connect(&nodes[i]);
    // Find the indegrees of each component
    for(int i = 1; i <= N; i++)
    {
        nodes[i].index = 0; // Prepare our dfs call, now index indicates previously visited
        for(auto n : nodes[i].v)
            if(n->component != nodes[i].component)
                in[n->component]++;
    }
    // Check if a root component can reach every other component
    for(int i = 1; i <= N; i++)
    {
        if(in[nodes[i].component] == 0)
        {
            if(dfs(&nodes[i]) == N)
                for(int j = 1; j <= N; j++)
                    if(nodes[j].component == nodes[i].component)
                        printf("%d ", j);
            printf("0");
            return 0;
        }
    }
}