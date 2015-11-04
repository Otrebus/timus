/* 1145. Rope in the Labyrinth - http://acm.timus.ru/problem.aspx?num=1145
 *
 * Strategy:
 * For some position in the maze, find the point furthest away from this point, and from the last
 * point do the same. The distance between these two latter points equal the result.
 *
 * Performance:
 * Linear in the size of the input, runs the test cases in 0.046s using 21792KB memory.
 */

#include <stdio.h>

unsigned char A[820][821];
int d, mi, mj, n, m;
int visited;

inline bool valid(short x, short y)
{
    return x >= 0 && x < m && y >= 0 && y < n && (A[x][y] & 0x7F) == '.';
}

void dfs(short x, short y, int dist)
{
    if(!valid(x, y) || ((A[x][y] >> 7) != visited))
        return;
    A[x][y] = (A[x][y] & 0x7F) | (~visited << 7);
    if(dist > d)
        d = dist, mi = x, mj = y;
    dfs(x-1, y, ++dist); dfs(x+1, y, dist); dfs(x, y-1, dist); dfs(x, y+1, dist);
}

int main()
{
    scanf("%d %d", &n, &m);
    for(int i = 0; i < m; i++)
        scanf("%s", A[i]);

    int si = -1, sj = 0;
    for(int i = 0; i < m && si < 0; i++)
        for(int j = 0; j < n && si < 0; j++)
            if(A[i][j] == '.')
                si = i, sj = j;
    dfs(si, sj, 0);
    visited = 1;
    dfs(mi, mj, 0);
    printf("%d", d);
    scanf("%d");
}