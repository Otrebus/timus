/* 1315. MDPAR and MIIAR - http://acm.timus.ru/problem.aspx?num=1315
 *
 * Strategy:
 * Do a simultaneous BFS from the air pockets, the surface and the speleologist (meaning these
 * positions are the initial members in the BFS queue), recording the shortest distance to any of
 * the above at each cell. We then do a DFS from the speleologists's cell and see if he can reach
 * the surface without passing through a position with distance exceeding D/2 (and not traversing
 * two D/2-depth positions in a row if D is even).
 *
 * Performance:
 * O(WH), runs the tests in 0.031s using 1,556KB memory.
 */

#include <stdio.h>
#include <queue>
#include <array>
#include <stack>

struct pos { // Used when BFSing 
    short x, y, d;
};

struct {
    short d; // The distance
    char v; // Bitfield that records whether this node is an air pocket, or has been visited during
            // the different iteration steps
} A[501][501];

short W, H, X, Y, D;
char s[502];

bool isValid(int x, int y) { // Whether it's valid to bfs/dfs into this cell
    return !(x < 1 || y < 1 || x > W || y > H || (A[x][y].v & 16));
}

pos va[4] = { { -1, 0 }, { 1, 0 }, { 0, -1 }, { 0, 1 } }; // Neighboring positions
pos vd[3] = { { -1, 0 }, { 1, 0 }, { 0, -1 } };           // Neighboring positions except up

int main() {
    scanf("%hu %hu %hu %hu %hu", &W, &H, &X, &Y, &D);

    for(int y = H; y >= 1; y--) {
        scanf("%s", s);
        for(int x = 1; x <= W; x++)
            A[x][y].v = s[x-1] == '#' ? 16 : 0;
    }

    // Initialize the airpocket-search BFS (from the water surface)
    std::queue<pos> Q;
    for(short x = 1; x <= W; x++)
        if(A[x][H].v ^ 16)
            Q.push( { x, H } );

    // Find air pockets by searching everywhere but up
    while(!Q.empty()) {
        auto p = Q.front();
        Q.pop();
        A[p.x][p.y].v |= 1;
        for(pos& dp : vd) {
            short x = dp.x + p.x, y = dp.y + p.y;
            if(!isValid(x, y) || (A[x][y].v & 2))
                continue;
            A[x][y].v |= 2;
            Q.push({ x, y });
        }
    }

    // Initialize the distance BFS
    for(short y = H+1; y >= 1; y--)
        for(short x = 1; x <= W; x++)
            if(y == H+1 || !(A[x][y].v & 16) && !(A[x][y].v & 1) || x == X && y == Y)
                A[x][y].v |= 4, Q.push({ x, y, 0 });

    // BFS from air pockets, the surface, and the player, recording the distance from each
    while(!Q.empty()) {
        auto p = Q.front();
        A[p.x][p.y].d = p.d;
        Q.pop();
        for(pos& dp : va) {
            short x = dp.x + p.x, y = dp.y + p.y;
            if(!isValid(x, y) || (A[x][y].v & 4))
                continue;
            Q.push({ x, y, p.d+1 });
            A[x][y].v |= 4;
        }
    }

    // Attempt to DFS to the surface while keeping ourselves at positions where d <= D/2
    std::stack<pos> S;
    S.push({ X, Y });
    while(!S.empty()) {
        auto p = S.top();
        S.pop();
        if(p.y == H)
            return printf("Can be rescued by himself"), 0;
        A[p.x][p.y].v |= 8;
        for(pos& dp : va) {
            short x = dp.x + p.x, y = dp.y + p.y;
            if(!isValid(x, y) || (A[x][y].v & 8))
                continue;
            A[x][y].v |= 8;
            short d = A[x][y].d, pd = A[p.x][p.y].d;
            if(d < D/2 || d == D/2 && (pd < D/2 || D%2))
                S.push({ x, y });
        }
    }
    printf("Rescue operation required");
}
