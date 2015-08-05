/* 1963. Kite - http://acm.timus.ru/problem.aspx?num=1963
 *
 * Strategy:
 * Geometry. This description will be somewhat nebulous due the time it would take to do
 * ascii drawings, but the comparison of angles is done using dot products and rearranging
 * the resulting equations to remove divisions, and comparison of lengths is, per usual, 
 * done on the squares of the lengths.
 *
 * Performance:
 * Constant.
 */

#include <stdio.h>
#include <algorithm>

int doAngle(int* x, int* y)
{
    long long xa = x[2]-x[3], xb = x[0]-x[3], xc = x[2]-x[1], xd = x[0]-x[1], 
              ya = y[2]-y[3], yb = y[0]-y[3], yc = y[2]-y[1], yd = y[0]-y[1];
    return int((xa*xb+ya*yb)*(xc*xc+yc*yc)*(xd*xd+yd*yd) ==
               (xc*xd+yc*yd)*(xa*xa+ya*ya)*(xb*xb+yb*yb) && (xa*xa+ya*ya) == (xc*xc+yc*yc));
}

int doTrap(int* x, int* y)
{
    long long xa = x[1]-x[0], xb = x[3]-x[0], xc = x[2]-x[3], xd = -xb;
    long long ya = y[1]-y[0], yb = y[3]-y[0], yc = y[2]-y[3], yd = -yb;
    return int((xa*xb+ya*yb)*(xc*xc+yc*yc)*(xd*xd+yd*yd) ==
               (xc*xd+yc*yd)*(xa*xa+ya*ya)*(xb*xb+yb*yb) && (xa*xa+ya*ya) == (xc*xc+yc*yc));
}

int main()
{
    int x[4];
    int y[4];
    scanf("%d %d %d %d %d %d %d %d", x, y, x+1, y+1, x+2, y+2, x+3, y+3);
    int sum = 0;
    int X[4], Y[4];
    for(int i = 0; i < 2; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            X[(j+i)%4] = x[j];
            Y[(j+i)%4] = y[j];
        }
        sum += doAngle(X, Y);
        sum += doTrap(X, Y);
    }
    printf("%d", sum*2);
    return 0;
}