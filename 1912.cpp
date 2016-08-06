/* 1912. Titan Ruins: Passing through Walls - http://acm.timus.ru/problem.aspx?num=1912
 *
 * Strategy:
 * Move each person in turn in a breadth-first fashion, while memoizing the best value for each 
 * position of person 1 and 2 (actually, the position of 1 and the relative position of 2) to
 * cut down the otherwise infinite search tree.
 *
 * Performance:
 * Hard to surmise, but likely close to O(nm) in practical cases; runs the tests in 0.124s using
 * 4,736KB memory.
 */

#include <limits>
#include <queue>
#include <cstring>
#include <stdio.h>

const int maxn = 250;

struct pos 
{ 
    int x, y; 
    bool operator==(const pos& p2) const { return x == p2.x && y == p2.y; }
    pos operator-(const pos& p2) const { return pos { x-p2.x, y-p2.y }; }
    pos operator+(const pos& p2) const { return pos { x+p2.x, y+p2.y }; }
};

int C[maxn+2][maxn+2][9]; // The cost of moving from [x][y] in the given direction
int A[maxn+2][maxn+2][9]; // The shortest path cost to [x][y] with person 2 at given relative pos
int n, m;
pos s1, s2;
const int oo = 0x7f7f7f7f;

const pos dirs[4] = { { -1, 0 }, { 0, -1 }, { 1, 0 }, { 0, 1 } };

// Packs the given relative position to a number 0-8
int pack9(pos p)
{
    return (1+p.y)*3 + 1+p.x;
}

// Unpacks the above
pos unpack9(int p)
{
    return { p%3-1, p/3-1 };
}

// Checks if someone is outside the maze
bool isOutside(pos p)
{
    return p.x == 0 || p.y == 0 || p.x == m || p.y == n;
}

// Checks that p1 and p2 are touching corners and are within the maze, etc
bool isValid(pos p1, pos p2)
{
    int dx = p2.x - p1.x, dy = p2.y - p1.y;
    return p1.x >= 0 && p2.x >= 0 && p1.x <= m && p2.x <= m &&
           p1.y >= 0 && p2.y >= 0 && p1.y <= n && p2.y <= n &&
           !(p1 == p2) && dx*dx + dy*dy <= 2;
}

int main()
{
    scanf("%d %d", &n, &m);
    std::memset(A, 0x7F, sizeof(A));

    // Read the input, fill in the necessary movement cost information
    for(int y = 1; y <= 2*n + 1; y++)
    {
        for(int x = 1; x <= 2*m + 1; x++)
        {
            char c = '\n';
            while(c == '\n' || c == '\r')
                scanf("%c", &c);
            if(c == '-')
                C[x/2][(y+1)/2][1] = C[x/2][y/2][7] = oo;
            else if(c == '|')
                C[(x+1)/2][y/2][3] = C[x/2][y/2][5] = oo;
            else if(c == '.')
            {
                if(y % 2 == 1)
                    C[x/2][(y+1)/2][1] = C[x/2][y/2][7] = 1;
                else
                    C[(x+1)/2][y/2][3] = C[x/2][y/2][5] = 1;
            }
            else if(c == '1')
                s1 = { x/2, y/2 };
            else if(c == '2')
                s2 = { x/2, y/2 };
        }
    }
    m++; n++;
    std::queue<std::pair<pos, pos>> q; // bfs q
    q.push( { s1, s2-s1 } );
    A[s1.x][s1.y][pack9(s2-s1)] = 0;

    while(!q.empty())
    {
        auto pr = q.front();
        auto p1 = pr.first, p2 = p1+pr.second;
        q.pop();

        for(pos p : dirs)
        {
            // Try moving person 1 first
            int door = C[p2.x][p2.y][pack9(p)]; // Cost of moving through this door, if any
            int cost = door + A[p1.x][p1.y][pack9(p2-p1)]; // Total cost of moving to the target pos
            if(door != oo && isValid(p1, p2+p) && A[p1.x][p1.y][pack9(p2+p-p1)] > cost)
                q.push( { p1, p2+p-p1 } ), A[p1.x][p1.y][pack9(p2+p-p1)] = cost;
            // Now person 2
            door = C[p1.x][p1.y][pack9(p)];
            cost = door + A[p1.x][p1.y][pack9(p2-p1)];
            if(door != oo && isValid(p1+p, p2) && A[p1.x+p.x][p1.y+p.y][pack9(p2-(p1+p))] > cost)
                q.push( { p1+p, p2-(p1+p) } ), A[p1.x+p.x][p1.y+p.y][pack9(p2-(p1+p))] = cost;
        }
    }

    // Search all possible positions where both players are outside the maze for an optimal value
    int min = oo;
    for(int x = 0; x <= m; x++)
    {
        for(int y = 0; y <= n; y++)
        {
            pos p1 { x, y };
            for(int dx = -1; dx <= 1; dx++)
            {
                for(int dy = -1; dy <= 1; dy++)
                {
                    pos p2 = p1 + pos { dx, dy };
                    if(isOutside(p1) && isOutside(p2) && isValid(p1, p2))
                        min = std::min(A[x][y][pack9(p2-p1)], min);
                }
            }
        }
    }
    if(min == oo)
        printf("Death");
    else
        printf("%d", min);
}