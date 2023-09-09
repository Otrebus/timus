/* 1351. Good Gnusmas - Dead Gnusmas - http://acm.timus.ru/problem.aspx?num=1351
 *
 * Strategy:
 * For each gnusmas, use the vector product of its position vector and the bounds vectors to make
 * sure it is within the bounds. There is a special case of when both bounds vectors are collinear
 * and the gnusmas is 180 degrees behind them to consider.
 *
 * Performance:
 * O(N), runs the test cases in 0.031s using 188 KB memory.
 */

#include <stdio.h>
#include <memory>

struct point
{
    int x, y;
    point(int x = 0, int y = 0) : x(x), y(y) { }
    int operator% (const point& p) { return x*p.y-p.x*y; } // Cross product
    int operator* (const point& p) { return x*p.x+y*p.y; } // Dot product
    point operator- (const point& p) { return point(x-p.x, y-p.y); }
    int dist2() { return x*x + y*y; }
};

int main()
{
    point c1, c2, v;
    int D, N;
    scanf("%d %d %d %d %d %d", &D, &c1.x, &c1.y, &c2.x, &c2.y, &N);
    while(N--)
    {
        scanf("%d %d", &v.x, &v.y);
        if(c1%v >= 0 && c2%v <= 0 && v.dist2() <= D*D && !(c1%c2 == 0 && c1*v < 0))
            printf("YES\n");
        else
            printf("NO\n");
    }
}