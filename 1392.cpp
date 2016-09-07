/* 1392. Dreaming of Stars - http://acm.timus.ru/problem.aspx?num=1392
 *
 * Strategy:
 * Naively link together the given planets with a disjoint-set forest structure.
 *
 * Performance:
 * O(n^2), runs the tests in 0.015s using 384KB memory.
 */

#include <numeric>
#include <algorithm>
#include <vector>
#include <iostream>
#include <array>

const int maxn = 1000;

struct point
{
    int x, y, z, r;
    point* parent;
    int rank; // For the union-by-rank heuristic
    point operator-(const point& p) { return point(x - p.x, y - p.y, z - p.z); }
    int length2() { return x*x + y*y + z*z; }
    point(int x = 0, int y = 0, int z = 0, int r = 0) : parent(this), x(x), y(y), z(z), r(r) { }
} points[maxn+1];

void link(point* a, point* b)
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

point* find(point* s) // Disjoint set find
{
    if(s != s->parent)
        s->parent = find(s->parent);
    return s->parent;
}

void join(point* a, point* b) // Disjoint set union 
{
    link(find(a), find(b));
}

int map[maxn]; // Maps the representative of a set to one of the below vectors
std::vector<int> v[maxn+1]; // The ids of the planets of each set

int main()
{
    std::ios::sync_with_stdio(false);
    int n, m = 0;
    std::cin >> n;
    auto sq = [](int a) { return a*a; };
    for(int i = 0; std::cin >> points[i].x >> points[i].y >> points[i].z >> points[i].r; i++)
        for(int j = 0; j < i; j++) // Group this planet with all others it overlaps
            if((points[i] - points[j]).length2() < sq(points[i].r + points[j].r))
                join(&points[i], &points[j]);
    for(int i = 0; i < n; i++)
    {
        int num = find(&points[i]) - &points[0];
        if(!map[num])
            (map[num] = ++m);
        v[map[num]].push_back(i);
    }
    for(int i = 1; i <= m; i++)
        std::sort(v[i].begin(), v[i].end());
    for(int i = 1; i <= m; i++)
    {
        std::cout << v[i][0];
        for(int j = 1; j < v[i].size(); j++)
            std::cout << ", " << v[i][j];
        std::cout << "\n";
    }
}