/* 1338. Automobiles - http://acm.timus.ru/problem.aspx?num=1338
 *
 * Strategy:
 * Do bfs from T to every other point, where we have a relaxation step in which the distance is tie
 * broken by the direction in which we enter the cell and where the new cell inherits the direction
 * from which we left T. Whenever we encounter a checkpoint, we add the inherited direction to the
 * output answer.
 *
 * Performance:
 * O(TWH), runs the tests in 0.187s using 7,032KB memory.
 */

#include <stdio.h>
#include <tuple>
#include <vector>
#include <queue>
#include <cstring>

struct node {
    std::pair<int, int> d; // Distance
    char t; // The type of the cell
    bool v; // Visited?
    int o;  // The original direction
} nodes[500][500];

struct pos {
    int x, y;
    pos operator+ (const pos& p) { return { x+p.x, y+p.y }; }
};

int W, H, M, T;
std::vector<pos> v; // The checkpoint positions
int ans[5]; // Number of north, south, east, west checkpoint exits that enter a checkpoint

void solve(int T) {
    // Start the bfs at the given checkpoint
    auto x = v[T].x, y = v[T].y;
    std::queue<pos> Q({{ x, y }});
    pos dirs[4] = {{ 0, -1 }, { 0, 1 }, { 1, 0 }, { -1, 0 }};
    nodes[x][y] = { { 0, 0 }, nodes[x][y].t, true, -1 };

    while(!Q.empty()) {
        auto p = Q.front();
        auto d = nodes[p.x][p.y].d;
        Q.pop();
        int i = 0;

        for(int i = 1; i <= 4; i++) { // For each direction
            // Check that we can move in the direction
            auto p2 = p + dirs[i-1];
            if(p2.x >= W || p2.x < 0 || p2.y >= H || p2.y < 0 || nodes[p2.x][p2.y].t == 0)
                continue;

            node& n = nodes[p2.x][p2.y]; // The new popped node
            int o = nodes[p.x][p.y].o;
            if(o == -1) // We are at T, initialize the initial direction
                o = n.o = i;

            if(!n.v) // Recurse to the node if we haven't visited it yet
                Q.push({ p2.x, p2.y });
            n.v = true;
            if(n.d > std::make_pair(d.first + 1, i)) { // Relaxation
                n.d = { d.first + 1, i };
                n.o = o; // Inherit the original direction
            }
        }
    }

    // For each checkpoint, add the original direction it inherited to the answer
    for(int i = 0; i < v.size(); i++)
        if(i != T)
            ans[nodes[v[i].x][v[i].y].o]++;
}

int main() {
    scanf("%d %d", &W, &H);

    char st[501];
    for(int j = 0; j < H; j++) {
        scanf("%s", st);
        for(int i = 0; i < W; i++)
            nodes[i][j].t = st[i] == '.' ? 0 : st[i] == '#' ? 1 : 2;
    }

    for(int j = H-1; j >= 0; j--) // Add all the checkpoints in order to an array
        for(int i = 0; i < W; i++)
            if(nodes[i][j].t == 2)
                v.push_back({i, j});

    scanf("%d", &M);
    for(int m = 1; m <= M; m++) {
        // Reset everything
        std::memset(ans, 0, sizeof(ans));
        for(int j = 0; j < H; j++)
            for(int i = 0; i < W; i++)
                nodes[i][j].d = { 10000000, 0 }, nodes[i][j].v = false, nodes[i][j].o = false;

        // Read and solve
        scanf("%d", &T);
        solve(T-1);
        printf("Experiment #%d: North: %d, South: %d, East: %d, West: %d\n",
               m, ans[1], ans[2], ans[3], ans[4]);
    }
}
