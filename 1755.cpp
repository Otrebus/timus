/* 1755. Cake - http://acm.timus.ru/problem.aspx?num=1755
 *
 * Strategy:
 * If we think of the domain as an (n,m) rectangle in the first quadrant of the x-y plane, both
 * players want to put the coordinate representing their slice in the top right corner. However,
 * if the first player does, the second will have him use the reflection of that point about the
 * origin instead. The solution then is to place the coordinate along the isoline of the graph
 * a_2*x + b_2*y that passes through the origin, at one of two extreme points.
 *
 * Performance:
 * Constant, runs the tests in 0.015s using 172KB memory.
 */

#include <stdio.h>

int main()
{
    double a1, b1, a2, b2, n, m;
    scanf("%lf %lf %lf %lf %lf %lf", &a1, &b1, &a2, &b2, &n, &m);
    if(b2*a2 == 0)
        printf("%.10lf %.10lf", a2 ? n/2 : n, b2 ? m/2 : m);
    else
        printf("%.10lf %.10lf", m*b2 < n*a2 ? (b2/a2*m+n)/2 : n, m*b2 < n*a2 ? 0 : (m-n*a2/b2)/2);
}