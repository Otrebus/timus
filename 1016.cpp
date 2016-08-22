/* 1016. Cube on the Walk - http://acm.timus.ru/problem.aspx?num=1016
 *
 * Strategy:
 * Naive depth-first search while memoizing the cube configuration and optimal value for each
 * location to cull branches in the search tree.
 *
 * Performance:
 * The complexity is hard to surmise, but the program runs the tests in 0.031s using 216KB memory.
 */

#include <algorithm>
#include <stdio.h>
#include <cstring>
#include <vector>

// Adjacent edge list of the cube, edge 0-3 belongs to the near face, edge 4-7 to the far face, etc.
// These edges belong to the faces in the order given in the problem, and the smallest number faces
// east and the successive edges are clockwise
char A[4*6] = { 14, 11, 20, 17, 12, 19, 22, 9, 13, 7, 21, 1, 
                4, 8, 0, 16, 15, 3, 23, 5, 2, 10, 6, 18 };
int min[8][8][4*6]; // Minimum cost to each configuration of the cube
int P[8][8][4*6]; // Parent graph
int cube[6]; // The numbers written on the cube
int opp[6] = { 1, 0, 4, 5, 2, 3 }; // Opposite side of each face
std::pair<int, int> dir[4] = { { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 } };

// The result of rotating the given orientation of the die d*90 degrees. The orientation is
// expressed as the edge facing east
int rotate(int i, int d)
{
    int f = i/4, s = i%4;
    return f*4 + (s + d)%4;
}

// The new orientation resulting from moving the die with the given orientation in the given
// direction. The direction is 0 towards the east, 1 to the north, etc
int move(int i, int d)
{
    if(d == 0)
        return A[rotate(i, 2)];
    else if(d == 1)
        return rotate(A[rotate(i, 3)], 3);
    else if(d == 2)
        return rotate(A[i], 2);
    return rotate(A[rotate(i, 1)], 1);
}

// Does a depth-first search of all configurations of the die from position (x, y) with orientation
// s and initial value val
bool dfs(int s, int x, int y, int val)
{
    if(val + cube[opp[s/4]] >= min[x][y][s])
        return false; // Optimal already reached, make sure we don't update the predecessor matrix
    min[x][y][s] = val + cube[opp[s/4]];
    for(int i = 0; i < 4; i++)
    {
        int dx = dir[i].first, dy = dir[i].second;
        if(x + dx < 0 || x + dx > 7 || y + dy < 0 || y + dy > 7)
            continue;
        if(dfs(move(s, i), x + dx, y + dy, min[x][y][s]))
            P[x+dx][y+dy][move(s,i)] = 1 + (i+2)%4; // New optimal in that direction, update P
    }
    return true;
}

int main()
{
    std::memset(min, 0x7f, sizeof(min));
    char s[3], e[3];
    scanf("%s %s", s, e);
    for(int i = 0; i < 6; i++)
        scanf("%d", &cube[i]);
    int sx = s[0] - 'a', sy = s[1] - '1';
    int ex = e[0] - 'a', ey = e[1] - '1';
    dfs(8, sx, sy, 0); // Die starts with top edge number 8 facing east.
    int ans = std::numeric_limits<int>::max();
    int anss = 0;
    for(int i = 0; i < 24; i++)
    {
        if(min[ex][ey][i] < ans)
        {
            ans = min[ex][ey][i];
            anss = i;
        }
    }
    printf("%d ", ans);
    
    // Print out the optimal path
    std::vector<std::pair<int, int>> v({std::pair<int, int>{ex,ey}});
    while (P[ex][ey][anss])
    {
        auto d = P[ex][ey][anss];
        ex += dir[d-1].first;
        ey += dir[d-1].second;
        anss = move(anss, d-1);
        v.push_back( { ex, ey } );
    }
    for(auto it = v.rbegin(); it < v.rend(); it++)
        printf("%c%d ", it->first+'a', it->second+1);
    std::system("pause");
}