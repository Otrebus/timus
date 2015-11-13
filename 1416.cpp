/* 1416. Confidential - http://acm.timus.ru/problem.aspx?num=1416
 *
 * Strategy:
 * The task is requesting the cost of the 2 smallest spanning trees. We find the solution by first
 * computing the MST, and then for each edge in this tree we compute the smallest spanning tree
 * not containing that edge. This works since the second smallest MST must have at least one edge
 * not in the smallest MST or else they would be equal.
 *
 * Performance:
 * O(n^3), running the tests in 0.499s using 2224KB memory.
 */

#include <stdio.h>
#include <vector>
#include <algorithm>

struct node
{
    node* parent;
    int rank; // For the union-by-rank heuristic
    std::vector<std::pair<node*, int>> neighbors;
    node() : parent(this), rank(0) { }
};

struct edge
{
    node* a, *b;
    bool mst;
    int weight;
    edge(node* a, node* b, int weight) : a(a), b(b), weight(weight) { mst = false; }
};

void link(node* a, node* b) // Disjoint set union
{
    if(a->rank > b->rank)
        b->parent = a;
    else
    {
        a->parent = b;
        if(a->rank == b->rank)
            b->rank++;
    }
}

node* find(node* s) // Disjoint set find
{
    if(s != s->parent)
        s->parent = find(s->parent);
    return s->parent;
}

void join(node* a, node* b) 
{
    link(find(a), find(b));
}

std::vector<node> nodes;
std::vector<edge> edges;

const int inf = 1000000000;
int m1 = inf, m2 = inf, N, M;

int mst(edge* n) // Calculates the mst of the graph, excluding edge n (if n != 0)
{
    int added = 0;
    int ans = 0;
    for(node& a : nodes)
        a.rank = 0, a.parent = &a;
    for(edge& e : edges)
    {
        if(n != &e)
        {
            if(find(e.a) != find(e.b))
            {
                added++;
                join(e.a, e.b);
                ans += e.weight;
                if(n == 0)
                    e.mst = true;
            }
        }
        if(added == N - 1)
            return ans;
    }
    return added == N - 1 ? ans : inf;
}

int main()
{
    scanf("%d %d", &N, &M);
    nodes.reserve(N+1);
    for(int i = 1; i <= N+1; i++)
    {
        nodes.push_back(node());
        nodes.back().parent = &nodes.back();
    }
    edges.reserve(M);
    for(int i = 0; i < M; i++)
    {
        int x, y, w;
        scanf("%d %d %d", &x, &y, &w);
        edges.emplace_back(&nodes[x], &nodes[y], w);
    }
    std::sort(edges.begin(), edges.end(), [] (edge& a, edge& b) { return a.weight < b.weight; });
    int ans = mst(0);
    int ans2 = inf;
    for(auto& e : edges)
    {
        if(e.mst)
            ans2 = std::min(ans2, mst(&e));
    }

    printf("Cost: %d\nCost: %d\n", ans ? ans : -1, ans2 == inf ? -1 : ans2);
    return 0;
}