/* 1643. Attack of the Dark Fortress - http://acm.timus.ru/problem.aspx?num=1643
 *
 * Strategy:
 * Use BFS from each of the two starting points to find the distance to each point around the target
 * point; if both starting points can reach such a point, their maximum distance is recorded for that
 * point, and the answer is the minimum of all those maximums, plus one for the actual attack.
 *
 * Performance:
 * Linear in the size of the input, runs the test cases in 0.015s using 732KB memory.
 */

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <queue>

struct region
{
    char type;
    int x, y, cost;
    bool visited;
};

bool isTeleporter(char type)
{
    return type >= 'A' && type <= 'Z';
}

int n, m;
region regions[100][100];
std::vector<region*> tp['Z'-'A'+1];

void add(std::queue<region*>& Q, region* to, int l)
{
    Q.push(to);
    to->cost = l;
    to->visited = true;
}

void handleTransition(std::queue<region*>& Q, int l, int x, int y)
{
    if(x < 0 || y < 0 || x >= m || y >= n)
        return;
    auto to = &(regions[x][y]);
    if(to->type == '#' || to->type == '*' || to->visited)
        return;
    add(Q, to, l);
    char c = to->type;
    if(isTeleporter(c))
        for(auto r : tp[to->type-'A'])
            if(r != to && !r->visited)
                add(Q, r, l);
}

void solve(region* start)
{
    std::queue<region*> Q;
    add(Q, start, 0);
    while(!Q.empty())
    {
        auto p = Q.front();
        int x = p->x, y = p->y;

        for(auto dx : { -1, 0, 1 })
            for(auto dy : { -1, 0, 1 } )
                if(dx || dy)
                    handleTransition(Q, p->cost+1, x+dx, y+dy);
        Q.pop();
    }
}

int main()
{
    int ans[3][3] = { -1, -1, -1, -1, -1, -1, -1, -1, -1 };
    std::cin >> n >> m;
    region* start1 = regions[0], *start2 = regions[0];
    int tx, ty;

    for(int i = 0; i < n; i++)
    {
        std::string line;
        std::cin >> line;
        for(int j = 0; j < m; j++)
        {
            auto c = line[j];
            region& r = regions[j][i];
            r.cost = -1, r.x = j, r.y = i, r.type = c;
            if(c == '$')
                start1 = &r;
            else if(c == '!')
                start2 = &r;
            else if(c == '*')
                tx = j, ty = i;
            else if(isTeleporter(c))
                tp[c-'A'].push_back(&r);
        }
    }

    solve(start1);
    // Save the results for the points surrounding the target point
    for(int di = -1; di <= 1; di++)
        for(int dj = -1; dj <= 1; dj++)
            if(tx+dj >= 0 && ty+di >= 0 && tx+dj < m && ty + di < n 
                          && regions[tx+dj][ty+di].type != '#' && (di || dj))
                ans[1+dj][1+di] = regions[tx+dj][ty+di].cost;
    for(int i = 0; i < n; i++)
        for(int j = 0; j < m; j++)
            regions[j][i].visited = false, regions[j][i].cost = -1;
    solve(start2);

    int res = std::numeric_limits<int>::max();
    for(int di = -1; di <= 1; di++)
    {
        for(int dj = -1; dj <= 1; dj++)
        {
            if(tx+dj >= 0 && ty+di >= 0 && tx+dj < m && ty + di < n 
                          && regions[tx+dj][ty+di].type != '#' && (di || dj))
            {
                int a = ans[1+dj][1+di];
                int b = regions[tx+dj][ty+di].cost;
                if(a != -1 && b != -1)
                    res = std::min(res, std::max(a, b));
            }
        }
    }
    if(res < std::numeric_limits<int>::max())
        std::cout << (res + 1) << std::endl;
    else
        std::cout << "Impossible" << std::endl;
}