/* 1301. Cube in Labyrinth - http://acm.timus.ru/problem.aspx?num=1301
 *
 * Strategy:
 * Depth-first search through the maze while recording the shortest path to each position for
 * each configuration of the cube, terminating any branch whenever a shorter path than the
 * current was reached for the position and configuration of the branch.
 *
 * Performance:
 * O(XY), runs the tests in 0.001s using 404KB memory.
 */

#include <algorithm>
#include <string>
#include <vector>
#include <iostream>
#include <stack>
#include <cstring>

int N[255]; // The neighboring edge of the index edge
unsigned int A[255][11][11]; // Smallest cost matrix
bool V[11][11][11][11]; // Visibility matrix
int m, n;
struct point { int x, y; };

// Checks if { sx, sy } -> { x, y } is a valid move
inline bool valid(int sx, int sy, int x, int y)
{
    return x <= m && y <= n && x > 0 && y > 0 && V[sx][sy][x][y];
}

// Returns the configuration given by rotating this configuration 90*deg degrees. A configuration
// is defined by the id of the leftmost edge of the top surface of the cube
inline int rotate(int s, int deg)
{
    return (s+deg)%4+(s&0xF0);
}

// Returns the face yielded by flipping this face along the given direction
inline int flip(int s, int dir)
{
    if(dir == 0) // Left
        return N[rotate(s, 2)];
    else if(dir == 1) // Up
        return rotate(N[rotate(s, 3)], 3);
    else if(dir == 2) // Right
       return rotate(N[s], 2); 
    else if(dir == 3) // Down
        return rotate(N[rotate(s, 1)], 1);

}

int main()
{
    std::ios::sync_with_stdio(false);
    std::string str;
    bool vertical;
    int a, b, c, d, x, y;
    std::memset(V, true, sizeof(V));
    std::memset(A, 0xFF, sizeof(A));

    // Defines the neighboring (semi-)edge for the 4*6 (semi-)edges of the cube. A face is the
    // alphabetic part and the edge is the numeric part
    N[0xa0] = 0xb1; N[0xa1] = 0xf3; N[0xa2] = 0xd1; N[0xa3] = 0xc1; N[0xd0] = 0xc2; N[0xd1] = 0xa2;
    N[0xd2] = 0xf2; N[0xd3] = 0xe2; N[0xc0] = 0xb2; N[0xc1] = 0xa3; N[0xc2] = 0xd0; N[0xc3] = 0xe1;
    N[0xb0] = 0xf0; N[0xb1] = 0xa0; N[0xb2] = 0xc0; N[0xb3] = 0xe0; N[0xe0] = 0xb3; N[0xe1] = 0xc3; 
    N[0xe2] = 0xd3; N[0xe3] = 0xf1; N[0xf0] = 0xb0; N[0xf1] = 0xe3; N[0xf2] = 0xd2; N[0xf3] = 0xa1;
    std::stack<std::pair<point, int>, std::vector<std::pair<point, int>>> s; // dfs stack

    // Read the input
    std::cin >> m >> n >> a >> b >> c >> d;
    while(std::cin >> str)
    {
        if(str[0] == 'v')
            vertical = true;
        else if(str[0] == 'h')
            vertical = false;
        else
        {
            x = std::stoi(str);
            std::cin >> y;
            if(vertical)
                V[x+1][y][x][y] = V[x][y][x+1][y] = false;
            else
                V[x][y][x][y+1] = V[x][y+1][x][y] = false;
        }
    }

    // Dfs
    const point dir[4] = { { -1, 0 }, { 0, 1 }, { 1, 0 }, { 0, -1 } };
    s.push( { { a, b }, 0xa0 } );
    A[0xa0][a][b] = 0;
    while(!s.empty())
    {
        auto p = s.top().first;
        auto side = s.top().second;
        s.pop();
        for(int i = 0; i < 4; i++)
        {
            auto d = dir[i];
            point pd = { p.x+d.x, p.y+d.y };
            if(valid(p.x, p.y, pd.x, pd.y))
            {
                auto newside = flip(side, i);
                if(A[newside][pd.x][pd.y] > A[side][p.x][p.y])
                {
                    A[newside][pd.x][pd.y] = A[side][p.x][p.y] + 1;
                    s.push( { { pd.x, pd.y }, newside } );
                }
            }
        }
    }
    unsigned int min = std::numeric_limits<unsigned int>::max();
    for(int i = 0; i < 4; i++)
        min = std::min(min, A[0xa0+i][c][d]);
    if(min == 0xffffffff)
        std::cout << "No solution";
    else
        std::cout << min;
}