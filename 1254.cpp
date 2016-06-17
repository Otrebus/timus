/* 1254. Die Hard - http://acm.timus.ru/problem.aspx?num=1254
 *
 * Strategy:
 * Repeated Dijkstra.
 *
 * Performance:
 * O(KNMlog (NM)), runs the tests in 0.904s using 296KB memory.
 */

#include <stdio.h>
#include <queue>
#include <functional>

const int maxn = 75;
const double inf = std::numeric_limits<double>::infinity();

struct Node
{
    double d; // Optimal cost
    bool open; // !building?
    bool v; // Visited - as in, optimal cost finalized?
} nodes[maxn+1][maxn+1];

double search(int sx, int sy, int dx, int dy, int N, int M) // Dijkstra
{
    using pin = std::pair<double, std::pair<int, int>>;
    std::priority_queue<pin, std::vector<pin>, std::greater<pin>> q;
    auto valid = [] (int x, int y, int N, int M) 
                    { return nodes[x][y].open && x > 0 && y > 0 && x <= N && y <= M; };
 
    for(int i = 1; i <= N; i++)
        for(int j = 1; j <= M; j++)
            nodes[i][j].d = { inf }, nodes[i][j].v = false;
    nodes[sx][sy].d = 0;

    q.push( { 0, { sx, sy } } );
    while(!q.empty())
    {
        auto p = q.top();
        q.pop();
        auto x = p.second.first, y = p.second.second;
        auto& node = nodes[x][y];
        if(x == dx && y == dy)
            return node.d;
        if(node.v)
            continue;
        node.v = true;
        for(int dx = -1; dx <= 1; dx++)
        {
            for(int dy = -1; dy <= 1; dy++)
            {
                int nx = x+dx, ny = y+dy;
                if(dy == 0 && dx == 0 || !valid(nx, ny, N, M))
                    continue;
                auto& n = nodes[nx][ny];
                double cost = std::sqrt(dx*dx+dy*dy);
                if(!n.v && n.d > node.d+cost)
                {
                    n.d = node.d + cost;
                    q.push( { n.d, { nx, ny } } );
                }
            }
        }
    }
    return inf;
}

int main()
{
    int M, N, K, sx, sy, dx, dy;
    double speed, ans = 0;
    char input[maxn+1];
    scanf("%d %d %d %lf", &N, &M, &K, &speed);
    for(int i = 1; i <= M; i++)
    {
        scanf("%s", input);
        for(int j = 1; j <= N; j++)
            nodes[j][i].open = input[j-1] == '.';
    }
    scanf("%d %d", &sx, &sy);
    while(K--)
    {
        scanf("%d %d", &dx, &dy);
        double result = search(sx, sy, dx, dy, N, M);
        if(result < inf)
        {
            ans += result;
            sx = dx;
            sy = dy;
        }
    }
    printf("%.2lf\n", ans/speed);
}