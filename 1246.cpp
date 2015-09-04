/* 1246. Tethered Dog - http://acm.timus.ru/problem.aspx?num=1246
 *
 * Strategy:
 * Check if the sum of the clockwise angle of all vertices (in the order given, between the previous
 * and current edge for each vertex) matches the interior or exterior sum of such angles.
 *
 * Performance:
 * Linear, runs in 0.187s using 3408KB memory.
 */

#include <stdio.h>
#include <vector>
#include <cmath>

const double pi = 3.141592653589793238462643383279502884;

struct point
{
    double x, y;
    point(double x, double y) : x(x), y(y) { }
    point() {}
};

std::vector<point> v;

double angle(int i)
{
    point p[3];
    for(int j = 0; j < 3; j++)
        p[j] = v[(v.size()+i-1+j)%v.size()];
    double d1x = p[0].x-p[1].x; double d1y = p[0].y-p[1].y;
    double d2x = p[2].x-p[1].x; double d2y = p[2].y-p[1].y;
    double k = d1x*d2y-d2x*d1y;
    double ang = std::acos(((d1x*d2x)+(d1y*d2y))/(std::sqrt((d1x*d1x+d1y*d1y)*(d2x*d2x+d2y*d2y))));
    return k < 0 ? 2*pi-ang : ang;
}

int main()
{
    int n;
    scanf("%d", &n);
    v.insert(v.begin(), n, point());
    for(int i = 0; i < n; i++)
        scanf("%lf %lf", &v[i].x, &v[i].y);
    double ang = 0.0;
    for(int i = 0; i < n; i++)
        ang += angle(i);
    if(std::abs((2*pi*n-(n-2)*pi) - ang) < std::abs((n-2)*pi - ang))
        printf("ccw\n");
    else
        printf("cw\n");
    return 0;
}