/* 1980. Road to Investor - http://acm.timus.ru/problem.aspx?num=1980
 *
 * Strategy:
 * Calculate the shortest time taken in the graph using Dijkstra for various excess speeds where
 * the latter is iterated over using binary search.
 * 
 * Performance:
 * O((n + m)log n log m), running the test cases in 0.124s using 2360KB memory.
 */

#include <stdio.h>
#include <queue>
#include <functional>
#include <cmath>
#include <limits>

struct edge
{
    int a, b;
    double s, l;
    int id;
};

struct node
{
    bool visited;
    double t;
    std::pair<int, node*> p;
    std::vector<edge> v;
};

node nodes[10001];

// Single source shortest path (minimizing arrival time), sp over the speed limit
double dijkstra(int n, double sp)
{
    for(int i = 1; i <= n; i++)
    {
        nodes[i].t = std::numeric_limits<double>::infinity();
        nodes[i].visited = false;
    }
    nodes[1].t = 0;
    std::priority_queue<std::pair<double, int>, std::vector<std::pair<double, int>>, 
                        std::greater<std::pair<double, int>>> q;
    q.push( { 0, 1 } );
    while(!q.empty())
    {
        auto p = q.top();
        q.pop();
        auto& n = nodes[p.second];
        if(n.visited)
            continue;
        for(auto& e : n.v)
        {
            node& m = nodes[e.b];
            if(n.t + (e.l)/(e.s+sp) < m.t)
            {
                n.visited = true;
                m.t = n.t + (e.l)/(e.s+sp);
                q.push({m.t, e.b});
                m.p = { e.id, &n };
            }
        }
    }
    return nodes[n].t;
}

double binary(int n, double l, double r, double wanted) // Binary search
{
    while(true)
    {
        double mid = l + (r-l)/2;
        double result = dijkstra(n, mid);
        if(std::abs(r-l) < 1e-7)
            return mid;
        else if(result >= wanted)
            l = mid;
        else
            r = mid;
    }
}

int main()
{
    int n, m;
    scanf("%d %d", &n, &m);
    std::priority_queue<std::pair<int, int>> q;
    for(int i = 0; i < m; i++)
    {
        int a, b; 
        double s, l;
        scanf("%d %d %lf %lf", &a, &b, &s, &l); 
        nodes[a].v.push_back( edge { a, b, s, l, i+1 } );
        nodes[b].v.push_back( edge { b, a, s, l, i+1 } );
    }
    double wt;
    scanf("%lf", &wt);
    auto p = binary(n, 0, 1e8, wt); // 1e8 is the maximum length of any road, so with a lowest T of
    std::vector<int> ans;           // 1 we might have to drive 1e8 above the speed limit at most
    for(node* node = &nodes[n]; node->p.first != 0; node = node->p.second)
        ans.push_back(node->p.first);
    printf("%lf %d\n", p, ans.size());
    for(auto it = ans.rbegin(); it < ans.rend(); it++)
        printf("%d ", *it);
    return 0;
}