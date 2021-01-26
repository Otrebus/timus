/* 2140. BitMazeCraft - http://acm.timus.ru/problem.aspx?num=2140
 *
 * Strategy:
 * BFS.
 *
 * Performance:
 * O(ABH), runs the tests in 0.078s using 5,232KB memory.
 */

#include <iostream>
#include <queue>
#include <string>
#include <vector>

struct coord {
    int x, y, z;
    coord operator+ (const coord& c2) { return { x+c2.x, y+c2.y, z+c2.z }; }
    coord operator- (const coord& c2) { return { x-c2.x, y-c2.y, z-c2.z }; }
    bool operator!() { return !(x || y || z); };
};

struct cell {
    bool visited;
    coord prev;
    char type;
};

const char block = 35, empty = 46;

cell C[51][51][51];

int A, B, H;

char get(const coord& c) {
    if(c.x > A || c.x < 1 || c.y > B || c.y < 1 || c.z > H || c.z < 1)
        return block;
    return C[c.x][c.y][c.z].type;
}

int main() {
    int sx, sy, sz, ex, ey, ez;
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::cin >> A >> B >> H;

    for(int z = 1; z <= H; z++)
        for(int x = 1; x <= A; x++)
            for(int y = 1; y <= B; y++)
                std::cin >> C[x][y][z].type;

    std::cin >> sx >> sy >> sz >> ex >> ey >> ez;

    std::queue<coord> q;

    q.push( { sx, sy, sz } );

    while(!q.empty()) {

        coord dz = { 0, 0, 1 }, dz2 = { 0, 0, 2 };
        coord c = q.front();
        q.pop();
        C[c.x][c.y][c.z].visited = true;

        coord d[4] = { { 1, 0, 0 }, { 0, 1, 0 }, { -1, 0, 0 }, { 0, -1, 0 } };

        std::vector<coord> valid;
        for(int i = 0; i < 4; i++) {
            coord c2 = c + d[i];

            // Walk
            if(get(c2 - dz) == block && get(c2) == empty)
                valid.push_back(c2);
            // Climb
            if(get(c+dz) == empty && get(c2+dz) == empty && get(c2) == block)
                valid.push_back(c2 + dz);
            // Drop
            if(get(c2) == empty && get(c2-dz) == empty && get(c2-dz2) == block)
                valid.push_back(c2 - dz);

            coord c3 = c2 + d[i];
            // Jump
            if(get(c2) == empty && get(c2-dz) == empty && get(c3) == empty && get(c3-dz) == block)
                valid.push_back(c3);
        }

        for(auto v : valid) {
            auto &dest = C[v.x][v.y][v.z];
            if(dest.visited || !!dest.prev)
                continue;

            dest.prev = c;
            q.push(v);
        }
    }

    auto end = C[ex][ey][ez];
    if(!end.visited) {
        std::cout << "NO";
        return 0;
    }
    std::cout << "YES" << std::endl;

    coord c = { ex, ey, ez };
    auto prev = end.prev;
    std::vector<std::string> out;

    while(c.x != sx || c.y != sy || c.z != sz) {
        auto a = C[c.x][c.y][c.z];
        auto dir = prev = c - a.prev;
        std::string s;
        if(dir.z < 0)
            s = "drop ";
        else if(dir.z > 0)
            s = "climb ";
        else if(std::abs(dir.x) > 1 || std::abs(dir.y) > 1)
            s = "jump ";
        else
            s = "walk ";
        s += (dir.y > 0 ? "east" : dir.y < 0 ? "west" : dir.x > 0 ? "south" : "north");
        out.push_back(s);
        c = a.prev;
    }
    std::cout << out.size() << std::endl;
    for(auto it = out.rbegin(); it < out.rend(); it++)
        std::cout << *it << std::endl;
}
