/* 1447. Portkey Network - http://acm.timus.ru/problem.aspx?num=1447
 *
 * Strategy:
 * We're looking to minimize Σc/Σd. Assume that Σc/Σd = x for some x > 0, meaning Σc - xΣd = 0. We
 * then assign the weight of each edge as c_i - x*d_i and find the minimum-weight spanning tree. If
 * this minimum weight turns out to be less than 0, then we've managed to construct a tree where
 * Σc/Σd < x, so we can recurse using binary search with a smaller x. Repeat until we find the final
 * x.
 *
 * Performance:
 * Runs the tests in 1.781s using 15,788KB memory.
 */

#include <stdio.h>
#include <vector>
#include <algorithm>

using ld = long double;

struct node
{
    int p;
    int r; // For the union-by-rank heuristic
} V[1001];

struct edge
{
    int a, b;
    ld w, d, c;
} E[500000];

void link(int an, int bn) { // Disjoint-set join for Kruskal
    auto& a = V[an], &b = V[bn];
    if(a.r > b.r)
        b.p = an;
    else {
        a.p = bn;
        if(a.r == b.r)
            b.r++;
    }
}

int find(int a) { // Disjoint-set find for Kruskal
    int s = a;
    if(s != V[s].p)
        V[s].p = find(V[s].p);
    return V[s].p;
}

void join(int a, int b) { // Disjoint-set join for Kruskal
    link(find(a), find(b));
}

int N, M, a, b;
ld w, d, c;

void reset(ld x) { // Sets the weights of the graph to the new distances
    for(int i = 1; i <= N; i++)
        V[i].p = i, V[i].r = 0;
    for(int i = 0; i < M; i++)
        E[i].w = E[i].c - x*E[i].d;
}

ld solve(ld x) { // Kruskal
    reset(x);
    std::sort(E, E+M, [] (edge& a, edge& b) { return a.w < b.w; });
    w = 0, d = 0, c = 0;
    int f = 0;
    for(int i = 0; i < M && f < N-1; i++) {
        auto& e = E[i];
        if(find(e.a) != find(e.b)) {
            join(e.a, e.b);
            w += e.w;
            d += e.d;
            c += e.c;
            f++;
        }
    }
    return w;
}

int main()
{
    scanf("%d %d", &N, &M);
    for(int i = 0; i < M; i++)
        scanf("%d %d %lf %lf", &E[i].a, &E[i].b, &E[i].d, &E[i].c);

    ld l = 0, r = 1e6;
    while(r - l > 1e-9) { // Binary search as described
        ld m = (r+l)/2;
        if(solve(m) < 0)
            r = m;
        else
            l = m;
    }
    printf("%.10lf", c/d);
}
