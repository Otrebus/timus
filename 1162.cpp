/* 1162. Currency Exchange - http://acm.timus.ru/problem.aspx?num=1162
 *
 * Strategy:
 * Bellman-Ford.
 *
 * Performance:
 * O(NM), runs in 0.015s using 440KB memory.
 */

#include <iostream>
#include <vector>
#include <algorithm>

struct edge
{
    int a, b;
    double r, c;
};

std::vector<edge> v;
double nodes[101];

int result(bool b)
{
    std::cout << (b ? "YES" : "NO") << "\n";
    return 0;
}

int main()
{
    int N, M, S;
    double V;
    std::cin >> N >> M >> S >> V;
    nodes[S] = V;
    for(int i = 0; i < M; i++)
    {
        int a, b;
        double rab, cab, rba, cba;
        std::cin >> a >> b >> rab >> cab >> rba >> cba;
        v.push_back( { a, b, rab, cab } );
        v.push_back( { b, a, rba, cba } );
    }
    for(int i = 0; i < N-1; i++)
        for(int j = 0; j < v.size(); j++)
            nodes[v[j].b] = std::max((nodes[v[j].a]-v[j].c)*v[j].r, nodes[v[j].b]);
    for(int i = 0; i < v.size(); i++)
        if((nodes[v[i].a]-v[i].c)*v[i].r > nodes[v[i].b] + 0.000001)
            return result(true);
    return result(false);
}