/* 1888. Pilot Work Experience - http://acm.timus.ru/problem.aspx?num=1888
 *
 * Strategy:
 * Create a graph where each pilot is a node and each pilot trip is an edge. Identify the components
 * of the graph and do a bfs from each node in each component to find a depth ordering of the nodes
 * such that the difference of depth of each pair of nodes is exactly equal to one. If, for any 
 * component this is not possible, return -1. If there is exactly one component of the graph, output 
 * the depths of its nodes. If there is more than one component, do the same, but "invert" the 
 * depths of one component (50->1, 49->2, etc) to maximize the total depth differences between all 
 * nodes.
 * 
 * Performance:
 * O(p(n+p)) since we're doing a breadth-first search from every node. The solution runs in 0.015s 
 * and uses 276 KB memory.
 */

#include <stdio.h>
#include <vector>
#include <algorithm>
#include <queue>

struct node
{
    std::vector<node*> edges;
    int depth, id;
    bool visited;
};

node nodes[51];
int ans[51];

std::vector<node*> makeComponent(node* start)
{   // Outputs a list of all the nodes of the component containing the given node
    std::vector<node*> ret;
    std::queue<node*> s;
    s.push(start);
    start->visited = true;
    while(!s.empty())
    {
        node* n = s.front();
        ret.push_back(n);
        s.pop();
        for(auto it = n->edges.begin(); it < n->edges.end(); it++)
        {
            node* v = *it;
            if(!v->visited)
            {
                v->visited = true;
                s.push(v);
            }
        }
    }
    return ret;
}

int test(node* start)
{   // Checks if a bfs from the start node produces a satisfying depth ordering and returns its
    // maximum depth
    for(int i = 0; i <= 50; i++)
        nodes[i].visited = false;
    std::queue<node*> s;
    s.push(start);
    start->visited = true;
    int max = 0;
    start->depth = 1;
    while(!s.empty())
    {
        node* n = s.front();
        s.pop();
        for(auto it = n->edges.begin(); it < n->edges.end(); it++)
        {
            node* v = *it;
            if(!v->visited)
            {
                v->visited = true;
                v->depth = n->depth + 1;
                max = std::max(n->depth+1, max);
                s.push(v);
            }
            else if(std::abs(v->depth-n->depth) != 1)
                return -1;
        }
    }
    return max;
}

void assign(node* start, int dir)
{   // Assigns the depths of the vertices of the given component to the answer array, from 1 to 50
    // or 50 to 1 depending on the direction given
    for(int i = 0; i <= 50; i++)
        nodes[i].visited = false;
    std::queue<node*> s;
    s.push(start);
    start->depth = dir > 0 ? 1 : 50;
    start->visited = true;
    while(!s.empty())
    {
        node* n = s.front();
        s.pop();
        ans[n->id] = n->depth;
        for(auto it = n->edges.begin(); it < n->edges.end(); it++)
        {
            node* v = *it;
            if(!v->visited)
            {
                v->depth = n->depth + dir;
                v->visited = true;
                s.push(v);
            }
        }
    }
}

int main()
{
    int n, p, ansmax = -1;
    std::vector<std::vector<node*>> components;

    scanf("%d %d", &n, &p);
    for(int i = 0; i <= p; i++)
        nodes[i].id = i;
    for(int i = 0; i < n; i++)
    {
        int x, y;
        scanf("%d %d", &x, &y);
        nodes[x].edges.push_back(nodes+y);
        nodes[y].edges.push_back(nodes+x);
    }
    for(int i = 1; i <= p; i++)
        if(!nodes[i].visited)
            components.push_back(makeComponent(nodes+i));
    for(auto it = components.begin(); it < components.end(); it++)
    {
        int max = -1;
        for(auto it2 = it->begin(); it2 < it->end(); it2++)
        {
            int result = test(*it2);
            if(result > max)
            { // If we already had a previous good component, reverse the direction for the next one
                assign(*it2, ansmax >= 0 ? -1 : 1);
                max = result;
            }
        }
        if(max < 0)
        {
            printf("-1\n");
            return 0;
        }
        else
            ansmax = max; // This component had a "good" depth ordering so take note of that
    }
    int min = 50, max = 0;
    for(int i = 1; i <= p; i++)
    {
        min = std::min(min, ans[i]);
        max = std::max(max, ans[i]);
    }
    printf("%d\n", max-min);
    for(int i = 1; i <= p; i++)
        printf("%d ", ans[i] );
    return 0;
}