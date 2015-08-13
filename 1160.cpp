/* 1160. Network - http://acm.timus.ru/problem.aspx?num=1160
 *
 * Strategy:
 * Kruskal.
 *
 * Performance:
 * O(M log M), runs the tests in 0.001s using 508 KB, which is the fastest submission to date for
 * some reason.
 */

#include <stdio.h>
#include <vector>
#include <algorithm>

struct node
{
    node* parent;
    int rank; // For the union-by-rank heuristic
    int n;  // Number of this node
    node() : parent(this), rank(0) { }
};

struct edge
{
    node* a, *b;
    int weight;
    edge(node* a, node* b, int weight) : a(a), b(b), weight(weight) {}
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
std::vector<edge*> ans;

int main()
{
    int N, M;
    scanf("%d %d", &N, &M);
    nodes.reserve(N+1);
    for(int i = 0; i < N+1; i++)
    {
        nodes.push_back(node());
        nodes.back().parent = &nodes.back();
        nodes.back().n = i;
    }
    edges.reserve(M);
    ans.reserve(M); 
    for(int i = 0; i < M; i++)
    {
        int x, y, w;
        scanf("%d %d %d", &x, &y, &w);
        edges.emplace_back(&nodes[x], &nodes[y], w);
    }
    std::sort(edges.begin(), edges.end(), [] (edge& a, edge& b) { return a.weight < b.weight; });
    int max = 0;
    for(edge& e : edges)
    {
        if(find(e.a) != find(e.b))
        {
            join(e.a, e.b);
            max = std::max(max, e.weight);
            ans.push_back(&e);
        }
    }
    printf("%d\n", max);
    printf("%d\n", ans.size());
    for(auto e : ans)
        printf("%d %d\n", e->a->n, e->b->n);
    return 0;
}