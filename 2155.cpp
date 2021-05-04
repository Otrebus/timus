/* 2155. Quadrocopter - http://acm.timus.ru/problem.aspx?num=2155
 *
 * Strategy:
 * Trace the path of the quadrocopter, recording the time it enters each square, and also, if it
 * loops, the second time it enters the square, to deduce the period of the loop. Calculate the
 * single-source shortest paths for Vova, recording the minimum time for him to reach each square.
 * Then, for each square, find the earliest time the quadrocopter enters the square after Vova does
 * (if any) and report the minimum time over all squares.
 *
 * Performance:
 * O(hw log hw), Runs the tests in 0.109s using 5,132KB memory.
 */

#include <stdio.h>
#include <set>

using ll = long long;
const ll inf = 1e16;
struct node {
    node() : dq(inf) {}
    ll dq, dq2, dv, c; // Time of first and second drone visits, earliest vova visit, swampiness
    char w; // Wind dir
    bool vv; // Vova visited?
    bool loop; // Revisiting in a loop
} nodes[251][251];

struct pos { int x, y; };
pos d[265];

int h, w;
int r1, c1, r2, c2;

void fly(int x, int y) {
    // Flies the quadrocopter along the wind
    int t = 0;
    while(x >= 0 && x < w && y >= 0 && y < h && !nodes[x][y].loop) {
        if(nodes[x][y].dq != inf) { // Already visited, this is a loop
            nodes[x][y].loop = true;
            nodes[x][y].dq2 = t;
        }
        else {
            nodes[x][y].dq = t;
        }
        auto dp = d[nodes[x][y].w]; // Figure out where the wind takes us
        x += dp.x, y += dp.y;
        t++;
    }
}

void dijkstra(int x, int y) {
    // Standard single-source shortest path for Vova
    std::set<std::pair<int, int>> S;
    S.insert({ 0, x+y*w });
    while(!S.empty()) {
        auto p = S.begin();
        auto d = p->first;
        auto x = p->second%w, y = p->second/w;
        S.erase(p);
        auto& node = nodes[x][y];
        if(node.vv)
            continue;
        node.dv = d;
        node.vv = true;
        for(auto dp : { pos{ 0, -1 }, { -1, 0 }, { 1, 0 }, { 0, 1 }}) {
            auto nx = x + dp.x, ny = y + dp.y;
            if(nx >= 0 && nx < w && ny >= 0 && ny < h) {
                S.insert({ node.dv + nodes[nx][ny].c, ny*w+nx });
            }

        }
    }
}

int main() {
    d['U'] = { 0, -1 }, d['L'] = { -1, 0 }, d['R'] = { 1, 0 }, d['D'] = { 0, 1 };
    scanf("%d %d %d %d %d %d", &h, &w, &r1, &c1, &r2, &c2);
    for(int y = 0; y < h; y++)
        for(int x = 0; x < w; x++)
            scanf("%d", &nodes[x][y].c);
    for(int y = 0; y < h; y++) {
        char r[300];
        scanf("%s", r);
        for(int x = 0; x < w; x++)
            nodes[x][y].w = r[x];
    }

    fly(c2-1, r2-1);
    dijkstra(c1-1, r1-1);

    auto m = inf;
    for(int y = 0; y < h; y++) {
        for(int x = 0; x < w; x++) {
            auto n = nodes[x][y];
            if(n.dv <= n.dq) { // If Vova arrives before the drone, he catches it when it arrives
                m = std::min(n.dq, m);
            } else if(n.loop) { // Otherwise if it loops he will catch it later
                auto dq = n.dq, dq2 = n.dq2, dv = n.dv;
                // Figure out the next flyby (the next integer >= dv that divides dq2-dq offset dq)
                m = std::min((dv-dq)%(dq2-dq) == 0 ? n.dv : n.dv + (dq2-dq) - (dv-dq)%(dq2-dq), m);
            }
        }
    }
    printf("%d", m == inf ? -1 : m);
}
