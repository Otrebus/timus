/* 2006. Kirill the Gardener - http://acm.timus.ru/problem.aspx?num=2006
 *
 * Strategy:
 * Run a BFS both from the start point and the end point to calculate the minimum distance to every
 * point from the endpoint and the startpoint. Also, map each distance from the start point
 * to a collection of cells with that distance. The cell that we block must necessarily lie on the
 * optimal path. If we block such a cell at distance d, every cell up to d still has the correct
 * distance from the start point, and every point from the endpoint has the correct distance up
 * until their distance d2 to the blocking point. We then start a BFS from the cells with distance
 * d from the start point until they reach a cell with distance d2 from the end point to calculate
 * the new distances. For some extra speed, we skip blocking any cell if it and its predecessor
 * both have two exits (meaning we're in a corridor).
 *
 * Performance:
 * Runs the tests in 0.046s using 9,020KB memory.
 */

#include <stdio.h>
#include <queue>
#include <vector>
#include <cstring>

struct cell {
    cell* p; // The previous cell on the optimal path
    int d, d2, d3; // The optimal distance from the start, end, and from the start when blocking
    bool b; // Whether the cell is clear
    int v;  // Visited in the vth bfs run
} C[1002][1002];

int n, m, sx, sy, ex, ey;

struct pos {
    int x, y;
    pos operator+ (const pos& p) const { return { x + p.x, y + p.y }; }
};

const pos dirs[4] { { -1, 0, }, { 1, 0 }, { 0, -1 }, { 0, 1 } };

int getX(cell* c) { return (c-&C[0][0])/1002; } // Gets the x coordinate of a cell
int getY(cell* c) { return (c-&C[0][0])%1002; }

std::vector<pos> L[100000]; // Maps a distance to the set of cells of this distance to the start

int count(cell* c) { // Counts the number of exits from a cell
    pos p = { getX(c), getY(c) };
    int ret = 0;
    for(auto dir : dirs) {
        auto p2 = p + dir;
        if(C[p2.x][p2.y].b)
            ret++;
    }
    return ret;
}

void bfs() { // Calculates the distance from the start to other cells, fills up L
    std::queue<pos> Q;

    Q.push({ sx,sy });
    L[0].push_back({sx, sy});
    C[sx][sy].v = 1;
    C[ex][ey].d = 1e9;

    // Standard bfs stuff
    while(!Q.empty()) {
        auto c = Q.front();
        Q.pop();

        for(auto dir : dirs) {
            auto p2 = c + dir;
            if(!C[p2.x][p2.y].v && C[p2.x][p2.y].b) {
                C[p2.x][p2.y].p = &C[c.x][c.y];
                C[p2.x][p2.y].v = 1;
                C[p2.x][p2.y].d = C[c.x][c.y].d + 1;
                L[C[p2.x][p2.y].d].push_back({ p2.x, p2.y });
                Q.push({p2.x, p2.y});
            }
        }
    }
}

void bfs2(int V) { // Calculates the distance from the end to other cells
    std::queue<pos> Q;

    Q.push({ex,ey});
    C[ex][ey].v = V;

    while(!Q.empty()) {
        auto c = Q.front();
        Q.pop();

        for(auto dir : dirs) {
            auto p2 = c + dir;
            if(C[p2.x][p2.y].b && C[p2.x][p2.y].v < V) {
                C[p2.x][p2.y].v = V;
                C[p2.x][p2.y].d2 = C[c.x][c.y].d2 + 1;
                Q.push({p2.x, p2.y});
            }
        }
    }
}

int bfs3(int V, int d, int d2) { // Combines the start and end distances as described in the header
    std::queue<pos> Q;

    // Push all the nodes with distance d from the start and initialize them with distance d3
    for(auto ps : L[d-1]) {
        if(C[ps.x][ps.y].b) {
            Q.push(ps);
            C[ps.x][ps.y].d3 = C[ps.x][ps.y].d;
            C[ps.x][ps.y].v = V;
        }
    }

    while(!Q.empty()) {
        auto c = Q.front();
        Q.pop();

        for(auto dir : dirs) {
            auto p2 = c + dir;

            if((!C[p2.x][p2.y].b || C[p2.x][p2.y].v == V)) // Blocked or already visited by us
                continue;

            if(C[p2.x][p2.y].d2 <= d2) {
                // We reached a known distance to the endpoint, so we can report a new minimal
                return 1 + C[c.x][c.y].d3 + C[p2.x][p2.y].d2;
            } else {
                // Continue the bfs
                C[p2.x][p2.y].v = V;
                C[p2.x][p2.y].d3 = C[c.x][c.y].d3 + 1;
                Q.push({ p2.x, p2.y });
            }
        }
    }
    return 1e9;
}

int main() {
    scanf("%d %d %d %d %d %d", &n, &m, &sy, &sx, &ey, &ex);
    char s[1001];
    std::vector<cell*> A;

    pos maxp = { 0, 0 };
    for(int y = 1; y <= n; y++) {
        scanf("%s", s);
        for(int x = 1; x <= m; x++) {
            C[x][y].b = s[x-1] != '#';
            if(C[x][y].b)
                maxp = { x, y },
                A.push_back(&C[x][y]);
        }
    }
   
    // Calculate the distances from the start point
    bfs();
    
    // Push all the cells on an optimal path into a vector
    std::vector<cell*> v;
    for(auto c = C[ex][ey].p; c->p; c = c->p)
        if(!(count(c) == 2 && count(c->p) == 2) || c->p == &C[sx][sy])
            v.push_back(c);

    // Calculate the distances from the end point
    int maxd = 0, V = 2;
    bfs2(V);

    for(auto c : v) {
        int x = getX(c), y = getY(c);

        C[x][y].b = false; // Add a blockage
        // Do a bfs from the cells with distance d to the cells with backward distance d2
        int dst = bfs3(++V, C[x][y].d, C[x][y].d2);
        if(dst > maxd) {
            maxd = dst;
            maxp = { x, y };
        }
        C[x][y].b = true; // Remove a blockage
    }

    printf("%d %d", maxp.y, maxp.x);
}
