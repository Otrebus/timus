/* 1923. Scary Politics. - http://acm.timus.ru/problem.aspx?num=1923
 *
 * Strategy:
 * Repeatedly do a flood fill with the given alliance number in the appropriate corner, and
 * then count the number of connected locations with the same alliance number from both corners.
 *
 * Performance:
 * O(mnl), runs at 0.001s and uses 312 KB memory.
 */

#include <stdio.h>
#include <cctype>
#include <initializer_list>
#include <cstring>

struct point
{
    int x, y;
    point(int x, int y) : x(x), y(y) { };
    point() {};
};

char a[52][52]; // Alliance matrix
bool b[52][52]; // Visitation matrix

int n, m;

int fill(point p, int alliance)
{
    int ans = 0; // Number of locations visited
    int c = a[p.x][p.y];
    a[p.x][p.y] = alliance;
    b[p.x][p.y] = true;
    for(point p2 : { point(p.x+1, p.y), point(p.x-1, p.y), 
                        point(p.x, p.y+1), point(p.x, p.y-1) })
    {   // For each direction, if that location has the same color and hasn't been visited ..
        if(a[p2.x][p2.y] == c && !b[p2.x][p2.y])
        {   // .. mark visited and recurse
            b[p2.x][p2.y] = true;
            ans += fill(p2, alliance);
        }
    }
    b[p.x][p.y] = false; // Unmark visited for future calls
    return ans + 1;
}

int main()
{
    int k;
    scanf("%d %d", &n, &m);
    std::memset(a, -1, sizeof(a)); // Everything we dont read input into is -1 so it's never visited
    for(int y = 1; y <= n; y++)
        for(int x = 1; x <= m; x++)
            do a[x][y] = getchar()-'0'; while(std::isspace(a[x][y]+'0')); // Read input
    scanf("%d", &k);
    for(int i = 0; i < k; i++)
    {
        int alliance;
        scanf("%d", &alliance);
        point p = i % 2 ? point(m, 1) : point(1, n); // Alternate southwest and northeast
        fill(p, alliance);
    }
    printf("%d\n%d\n", fill(point(1, n), -2), fill(point(m, 1), -2));
    return 0;
}

/* 

// Clunky non-recursive solution below

#include <stdio.h>
#include <cctype>
#include <initializer_list>

struct point
{
    int x, y;
    point(int x, int y) : x(x), y(y) { };
    point() {};
};

char a[50][50] = { -1 };
bool b[50][50] = { false };

point s[50*50];
point t[50*50];

bool inbounds(int n, int m, point p)
{
    return p.x >= 0 && p.y >= 0 && p.x < m && p.y < n;
}

int main()
{
    int n, m, k;
    char str[51];
    scanf("%d %d", &n, &m);

    for(int y = 0; y < n; y++)
    {
        for(int x = 0; x < m; x++)
        {
            char c;
            do c = getchar(); while(std::isspace(c));
            a[x][y] = c-'0';
        }
    }
    scanf("%d", &k);
    for(int i = 0; i < k; i++)
    {
        int alliance;
        scanf("%d", &alliance);
        int tops = 0, topt = 0;
        point p = i % 2 ? point(m-1, 0) : point(0, n-1);
        int c = a[p.x][p.y];
        s[tops++] = t[topt++] = p;
        b[p.x][p.y] = true;
        while(tops != 0)
        {
            point p = s[--tops];
            a[p.x][p.y] = alliance;
            for(point p2 : { point(p.x+1, p.y), point(p.x-1, p.y), 
                             point(p.x, p.y+1), point(p.x, p.y-1) })
            {
                if(inbounds(n, m, p2) && !b[p2.x][p2.y] && a[p2.x][p2.y] == c)
                {
                    s[tops++] = p2;
                    t[topt++] = p2;
                    b[p2.x][p2.y] = true;
                }
            }
        }
        while(topt--)
            b[t[topt].x][t[topt].y] = false;
    }
    for(int i = 0; i < 2; i++)
    {
        int ans = 0;
        int tops = 0;
        point p = i % 2 ? point(m-1, 0) : point(0, n-1);
        int c = a[p.x][p.y];
        s[tops++] = p;
        b[p.x][p.y] = true;
        while(tops != 0)
        {
            point p = s[--tops];
            ans++;
            for(point p2 : { point(p.x+1, p.y), point(p.x-1, p.y), 
                             point(p.x, p.y+1), point(p.x, p.y-1) })
            {
                if(inbounds(n, m, p2) && !b[p2.x][p2.y] && a[p2.x][p2.y] == c)
                {
                    if(inbounds(n, m, p2) && !b[p2.x][p2.y] && a[p2.x][p2.y] == c)
                    {
                        b[p2.x][p2.y] = true;
                        s[tops++] = p2;
                    }
                }
            }
        }
        printf("%d\n", ans);
    }
    return 0;
}

*/