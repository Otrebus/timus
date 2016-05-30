/* 1325. Dirt - http://acm.timus.ru/problem.aspx?num=1325
 *
 * Strategy:
 * Dijkstra.
 *
 * Performance:
 * O(MN log(MN)), runs in 0.078s using 6,520KB memory.
 */

#include <sstream>
#include <stdio.h>
#include <queue>
#include <functional>

const int maxn = 500, inf = 1000000000;

struct Node
{
    std::pair<int, int> d; // Optimal cost to this node as in { shoes, distance }
    char a; // Type of this square
    bool v; // Visited - as in, optimal cost finalized?
} nodes[maxn+1][maxn+1];

void search(int sx, int sy, int M, int N) // Dijkstra
{
    using pin = std::pair<std::pair<int, int>, std::pair<int, int>>;
    std::priority_queue<pin, std::vector<pin>, std::greater<pin>> q;
    auto inside = [] (int x, int y, int M, int N) { return x > 0 && y > 0 && x <= N && y <= M; };
 
    for(int i = 1; i <= 500; i++)
        for(int j = 1; j <= 500; j++)
            nodes[i][j].d = { inf, 0 }, nodes[i][j].v = false;
    nodes[sx][sy].d = { 0, 1 };

    q.push( { { 0, 1 }, { sx, sy } } );
    while(!q.empty())
    {
        auto p = q.top();
        q.pop();
        auto x = p.second.first, y = p.second.second;
        auto& node = nodes[x][y];
        if(node.v)
            continue;
        node.v = true;
        for(int dx = -1; dx <= 1; dx++)
        {
            for(int dy = -1; dy <= 1; dy++)
            {
                int nx = x+dx, ny = y+dy;
                if(dy == 0 && dx == 0 || !inside(nx, ny, M, N) || nodes[nx][ny].a == 0)
                    continue;
                auto& n = nodes[nx][ny];
                int cost = (n.a != nodes[x][y].a) ? 1 : 0;
                if(!n.v && n.d > std::make_pair(node.d.first + cost, node.d.second + 1))
                {
                    n.d = std::make_pair(node.d.first + cost, node.d.second + 1);
                    q.push( { n.d, { nx, ny } });
                }
            }
        }
    }
}

int main()
{
    int M, N, sx, sy, dx, dy;
    char input[maxn+1];
    scanf("%d %d %d %d %d %d", &M, &N, &sy, &sx, &dy, &dx);
    for(int i = 1; i <= M; i++)
    {
        scanf("%s", input);
        for(int j = 1; j <= N; j++)
            nodes[j][i].a = input[j-1]-'0';
    }
    search(sx, sy, M, N);
    std::stringstream sout;
    if(nodes[dx][dy].d.first == inf)
        printf("0 0\n");
    else
        printf("%d %d", nodes[dx][dy].d.second, nodes[dx][dy].d.first);
}