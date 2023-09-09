/* 2113. Survive the flood - http://acm.timus.ru/problem.aspx?num=2113
 *
 * Strategy:
 * Assume some jump height and do a flood-filling breadth-first search on the graph starting from
 * two points - the initial positions of the water and the rabbit - where the water and the rabbits
 * are able to spread to the specified cells, and where the water "eats" every filled rabbit cell.
 * If there is some rabbit cell left after the game is finished (finished meaning the BFS queue is
 * empty), the rabbit wins. Perform the above with a binary search over all jump heights.
 *
 * Performance:
 * O(nm log h), runs the tests in 0.031s using 248KB memory.
 */

#include <stdio.h>
#include <queue>

int n, m;

struct vec {
    int x, y;
    int r; // The player (water or rabbit)
    vec operator+ (const vec& v) const { return { v.x+x, v.y+y, v.r+r }; }
};

vec dirs[4] = { { -1, 0 }, { 1, 0 }, { 0, -1 }, { 0, 1 } };
vec r, w; // Rabbit and water starting positions

struct {
    int h, v; // The height and filler of the cell
} nodes[102][102];

bool solve(int k) {
    for(int x = 1; x <= m; x++) // All cells are unoccupied at first
        for(int y = 1; y <= n; y++)
            nodes[x][y].v = 0;

    std::queue<vec> Qr({ r, w }); // The game starts with the two participants
    nodes[r.x][r.y].v = 2, nodes[w.x][w.y].v = 1; // If this square got flooded before we could pop
    while(!Qr.empty()) {
        vec p = Qr.front();
        Qr.pop();
        auto& n = nodes[p.x][p.y];
        if(p.r == 2 && n.v == 1) // The rabbit can't enter water
            continue;
        for(auto d : dirs) {
            auto p2 = p + d;
            auto &n2 = nodes[p2.x][p2.y];
            // All the game rules here
            if(n2.v == 1 || p.r == 1 && n2.h > n.h || p.r == 2 && (n2.v == 2 || n2.h-n.h > k))
                continue;
            n2.v = p.r; 
            Qr.push(p2);
        }
    }

    for(int x = 1; x <= m; x++) // If there are any rabbit cells, return true
        for(int y = 1; y <= n; y++)
            if(nodes[x][y].v == 2)
                return true;
    return false;
}

int binary() { // Binary search over the jump height
    int L = 0, R = 10e5 + 1;
    for(int m = (R+L)/2; R-L > 0; m = (R+L)/2)
        if(solve(m)) R = m; else L = m+1;
    return L;
}

int main() {
    scanf("%d %d", &n, &m);
    for(int y = 1; y <= n; y++)
        for(int x = 1; x <= m; x++)
            scanf("%d", &nodes[x][y].h);

    for(int x = 0; x <= m+1; x++) // Pretend the edges of the map are water to constrain the game
        for(int y = 0; y <= n+1; y++)
            nodes[x][y].v = 1;

    scanf("%d %d %d %d", &r.y, &r.x, &w.y, &w.x);
    r.r = 2, w.r = 1;
    int res = binary();
    printf("%d", res <= 10e5 ? res : -1);
}
