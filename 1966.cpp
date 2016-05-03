/* 1966. Cycling Roads - http://acm.timus.ru/problem.aspx?num=1966
 *
 * Strategy:
 * Calculate segment intersections, turn the corresponding topology into a graph, do dfs.
 *
 * Performance:
 * O(n^2), runs the test data in 0.015s using 620KB memory.
 */

#include <stdio.h>
#include <vector>
#include <algorithm>

struct point
{ 
    long long x, y; 
    point(long long x = 0, long long y = 0) : x(x), y(y) {}
    point operator-(point p) { return point(x-p.x, y-p.y); }
    long long operator%(point p) { return p.x*y-p.y*x; } // Cross product
};

struct node
{
    point p;
    bool visited;
    std::vector<node*> neighbors;
};

// Are the points v1 and v2 on opposite sides of the line going through the origin and u?
bool straddles(point u, point v1, point v2)
{
    auto a = u%v1, b = u%v2;
    return (u%v1 >= 0 && u%v2 <= 0 || u%v1 <= 0 && u%v2 >= 0) && (a != 0 || b != 0);
}

// Checks if point v is on the line segment between u1 and u2
bool intersect(point u1, point u2, point v)
{
    return (u2-u1) % (v-u1) == 0 && v.x >= std::min(u1.x, u2.x) && v.x <= std::max(u1.x, u2.x)
         && v.y >= std::min(u1.y, u2.y) && v.y <= std::max(u1.y, u2.y);
}

// Do the line segments [u1,u2] and [v1,v2] intersect?
bool intersect(point u1, point u2, point v1, point v2)
{
     return straddles(u2-u1, v1-u1, v2-u1) && straddles(v2-v1, u1-v1, u2-v1);
}

int A[201][201];
node nodes[201];
std::vector<std::pair<node*, node*>> lines; // Should be named "segments" but whatever

void connect(int a, int b)
{
    A[a][b] = A[b][a] = true;
}

void dfs(node* node)
{
    for(auto n : node->neighbors)
        if(!n->visited)
            n->visited = true, dfs(n);
}

int main()
{
    int m, n, i, j;
    scanf("%d %d", &n, &m);
    for(int l = 1; l <= n; l++)
    {
        scanf("%lld %lld", &nodes[l].p.x, &nodes[l].p.y);
    }

    while(m--)
    {
        scanf("%d %d", &i, &j);
        connect(i, j);
        for(auto l : lines) // Check if this segment connects to the earlier segments
        {
            auto a = l.first, b = l.second;
            if(intersect(a->p, b->p, nodes[i].p, nodes[j].p))
            {
                int ai = a-nodes, bi = b-nodes;
                connect(ai, i), connect(bi, i), connect(ai, j), connect(bi, j);
            }
        }
        for(int k = 1; k <= n; k++) // Check if this line passes any statues
        {
            if(intersect(nodes[i].p, nodes[j].p, nodes[k].p))
                connect(i, k), connect(j, k);
        }
        lines.push_back( { nodes+i, nodes+j } );
    }
    for(int i = 1; i <= n; i++)
        for(int j = 1; j <= n; j++)
            if(A[i][j] && i != j)
                nodes[i].neighbors.push_back(nodes+j);
    nodes[1].visited = true;
    dfs(nodes+1);
    for(int i = 1; i <= n; i++)
    {
        if(!nodes[i].visited)
        {
            printf("NO\n");
            return 0;
        }
    }
    printf("YES\n");
    return 0;
}