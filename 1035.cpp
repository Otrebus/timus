/* 1035. Cross-stitch - http://acm.timus.ru/problem.aspx?num=1035
 *
 * Strategy:
 * The problem asks us to create a graph with the minimum number of Euler paths. The algorithm to
 * extract such graphs from a given graph (see 1441.cpp) shows that we want as few vertices with
 * odd degree as possible, and as few components as possible overall. This leads us to join any
 * endpoints wherever we can to minimize the number of loose ends (vertices with an odd degree),
 * and then to reduce the number of graph components by merging any pair of components, of which at
 * least one is looping, at any vertex where they touch at an internal point.
 * 
 * Performance:
 * O(NM), runs the tests in 0.062s using 13,016KB memory.
 */

#include <stdio.h>
#include <vector>
#include <unordered_set>

const int maxn = 201;

struct comp { // A graph component
    comp* parent; // Disjoint-set parent
    int rank;     // Disjoint-set rank
    bool loop;    // Whether this component is a loop
} comps[maxn*maxn*8];

struct point { // An endpoint of a component
    comp* comp; // It's component
    bool linked; // Whether this endpoint is internal (has already been linked)
} points[maxn*maxn*8];
int c, e, p, n, m;

std::unordered_set<comp*> S; // Helps us count the number of components
std::vector<point*> P[maxn][maxn][2]; // The endpoints at each stitch point

comp* find(comp* s) // Finds the set representative of component s
{
    if(s != s->parent)
        s->parent = find(s->parent);
    return s->parent;
}

void join(comp* a, comp* b) // Joins components a and b, union-set style
{
    a = find(a), b = find(b);
    if(a->rank > b->rank)
        b->parent = a;
    else
        a->parent = b;
    if(a->rank == b->rank)
        b->rank++;
    a->loop = b->loop = a == b || a->loop && b->loop; // Set the looping status correctly
}

void link(point* p1, point* p2) { // Joins endpoints p1 and p2
    if(p1->linked || p2->linked)
        return;
    p1->linked = p2->linked = true;
    auto c1 = find(p1->comp), c2 = find(p2->comp);
    join(c1, c2);
}

void merge(point* u, point* v) { // Joins two points if one of their components have a cycle
    auto c1 = find(u->comp), c2 = find(v->comp);
    if(c1 != c2 && (c1->loop || c2->loop))
        join(c1, c2);
}

void link(int x1, int y1, int x2, int y2, int k) { // Adds a /, \, X to the graph
    auto& cp = comps[c++];

    auto& pt = points[p++], &pt2 = points[p++]; // Each stitch has two endpoints
    pt.comp = pt2.comp = cp.parent = &cp;

    for(auto& v : P[x1][y1][k^1]) // Link each to the backside if we can
        link(&pt, v);
    P[x1][y1][k].push_back(&pt);

    for(auto& v : P[x2][y2][k^1])
        link(&pt2, v);
    P[x2][y2][k].push_back(&pt2);
}

int main() {
    char s[201];
    scanf("%d %d", &n, &m);

    for(int k = 0; k < 2; k++) {
        for(int y = 0; y < n; y++) {
            scanf("%s", s);
            for(int x = 0; x < m; x++) { // Link endpoints as we read them
                if(s[x] == '/' || s[x] == 'X')
                    link(x, y+1, x+1, y, k);
                if(s[x] == '\\' || s[x] == 'X')
                    link(x, y, x+1, y+1, k);
            }
        }
    }

    for(int y = 0; y <= n; y++) // Merge loops
        for(int x = 0; x <= m; x++)
            for(auto u : P[x][y][0])
                for(auto v : P[x][y][1])
                    merge(u, v);

    for(int i = 0; i < c; i++) // Count the total number of components
        S.insert(find(&comps[i]));

    printf("%d", S.size());
}
