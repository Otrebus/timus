/* 2067. Friends and Berries - http://acm.timus.ru/problem.aspx?num=2067
 *
 * Strategy:
 * Doing the math, the problem asks to, if the input "coordinates" are in a single line, find the
 * two outermost points.
 *
 * Performance:
 * Linear in the size of the input, runs the test cases in 0.187s using 180KB memory.
 */

#include <stdio.h>
#include <cctype>
#include <algorithm>

struct pos
{
    int id;
    int p[2];
};

inline int getint()
{
    int ret = 0, ch, minus = 1;
    while(!isdigit(ch = getc(stdin)))
        if(ch == '-')
            minus = -1;
    ret = ch - '0';
    while(isdigit(ch = getc(stdin)))
        ret = ret*10 + ch - '0';
    return ret*minus;
}

int main()
{
    int max, min;
    int N = getint();
    pos a { 1, { getint(), getint() } };
    pos b { 2, { getint(), getint() } };
    int x = a.p[0] == b.p[0] ? 1 : 0;
    int y = (x + 1) % 2;
    long long dy = a.p[y] - b.p[y];
    long long dx = a.p[x] - b.p[x];
    int xmax = std::max(a.p[x], b.p[x]);
    int xmin = std::min(a.p[x], b.p[x]);
    max = b.p[x] == xmax ? 2 : 1;
    min = b.p[x] == xmin ? 2 : 1;
    for(int i = 3; i <= N; i++)
    {
        int q[2] = { getint(), getint() };
        if(dy*(q[x]-b.p[x]) != dx*(q[y]-b.p[y]))
        {
            printf("0\n");
            return 0;
        }
        if(q[x] < xmin)
        {
            min = i;
            xmin = q[x];
        }
        if(q[x] > xmax)
        {
            max = i;
            xmax = q[x];
        }
    }
    printf("1\n%d %d\n", min, max);
}