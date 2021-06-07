/* 1250. Sea Burial - http://acm.timus.ru/problem.aspx?num=1250
 *
 * Strategy:
 * Every contiguous body of land or water A that neighbors another body B is either completely
 * surrounded by, or completely surrounds B. We can then do a depth-first search from the outermost
 * body of land (that surrounds the input field) into any seas that it contains, and from those seas
 * into the islands that they contain, and so on recursively to calculate how many bodies of water
 * each land mass (or further out, sea) contains.
 *
 * Performance:
 * Linear in the size of the input, runs the test cases in 0.031s using 19,456KB memory.
 */

#include <stdio.h>
#include <vector>

#pragma comment(linker, "/STACK:16777216")

const int maxw = 500;
int w, h, sx, sy, m[maxw*maxw];
std::pair<int, int> dir[8] = {{-1, 0}, {0, -1}, {0, 1}, {1, 0}, {-1, 1}, {-1, -1}, {1, 1}, {1, -1}};

struct Tile
{
    char type;
    bool visited;
    int zone; // We enumerate each body so we can retrieve the result for the drowning point easily
} tiles[maxw+2][maxw+2];

// Does depth-first-search inside a contiguous body and populates the v array with the positions
// of any neighboring bodies
void fill(int x, int y, std::vector<std::pair<int, int>>& v, int& zone)
{
    int np = tiles[x][y].type == '#' ? 4 : 8;
    for(int i = 0; i < np; i++)
    {
        auto nx = dir[i].first + x, ny = dir[i].second + y;
        if(nx < 0 || nx > w || ny < 0 || ny > h || tiles[nx][ny].visited)
            continue;
        else if(tiles[nx][ny].type != tiles[x][y].type)
            v.push_back( { nx, ny } );
        else
        {
            tiles[nx][ny].zone = tiles[x][y].zone;
            tiles[nx][ny].visited = true;
            fill(nx, ny, v, zone);
        }
    }
}

// Does depth-first search from the body given by the coordinates and any unvisited bodies
// that it is adjacent to, while assuming that those unvisited bodies are strictly inside this body.
// Returns the total number of water bodies that this body contains directly or indirectly
int dfs(int x, int y, int& zone)
{
    int ret = tiles[x][y].type == '#';
    std::vector<std::pair<int, int>> v;
    fill(x, y, v, zone);
    for(auto p : v)
    {
        if(!tiles[p.first][p.second].visited)
        {
            tiles[p.first][p.second].visited = true;
            tiles[p.first][p.second].zone = ++zone;
            ret += dfs(p.first, p.second, zone);
        }
    }
    m[tiles[x][y].zone] = ret;
    return ret;
}

int main()
{
    // Fill in the border surrounding everything (well, not just the border)
    for(int y = 0; y < maxw+2; y++)
        for(int x = 0; x < maxw+2; x++)
            tiles[x][y].type = '#';

    scanf("%d %d %d %d", &w, &h, &sx, &sy);
    char str[501];
    for(int y = 1; y <= h; y++)
    {
        scanf("%s", str);
        for(int x = 1; x <= w; x++)
            tiles[x][y].type = str[x-1];
    }

    w++; h++;
    tiles[0][0].visited = true;
    int zone = 1;
    // We do depth-first-search from the outside border; that way we guarantee that each new
    // body that we encounter will be strictly inside the ones we visited so far
    dfs(0, 0, zone);
    printf("%d", m[tiles[sx][sy].zone]);
}