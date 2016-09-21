/* 1030. Titanic - http://acm.timus.ru/problem.aspx?num=1030
 *
 * Strategy:
 * Straightforward calculation. See e.g. https://en.wikipedia.org/wiki/Great-circle_distance
 *
 * Performance:
 * Constant, runs the tests in 0.001s using 252KB memory.
 */

#include <cmath>
#include <iostream>
#include <stdio.h>

int main()
{
    double x1, x2, x3, y1, y2, y3, a1, a2, a3, b1, b2, b3;
    char l[4];
    scanf("Message #%*d. Received at %*d:%*d:%*d.%*[\n\r ]");
    scanf("Current ship's coordinates are%*[\n\r ]");
    scanf("%lf^%lf'%lf\" %cL%*[\n\r ]", &x1, &x2, &x3, &l[0]);
    scanf("and %lf^%lf'%lf\" %cL.%*[\n\r ]", &y1, &y2, &y3, &l[1]);
    scanf("An iceberg was noticed at%*[\n\r ]");
    scanf("%lf^%lf'%lf\" %cL%*[\n\r ]", &a1, &a2, &a3, &l[2]);
    scanf("and %lf^%lf'%lf\" %cL.%*[\n\r ]", &b1, &b2, &b3, &l[3]);
    const double pi = std::acos(-1.0);
    double p1 = (x1 + x2/60 + x3/3600)*(l[0] == 'N' ? 1 : -1)*pi/180;
    double l1 = (y1 + y2/60 + y3/3600)*(l[1] == 'E' ? 1 : -1)*pi/180;
    double p2 = (a1 + a2/60 + a3/3600)*(l[2] == 'N' ? 1 : -1)*pi/180;
    double l2 = (b1 + b2/60 + b3/3600)*(l[3] == 'E' ? 1 : -1)*pi/180;
    double d = std::acos(std::sin(p1)*std::sin(p2) 
             + std::cos(p1)*std::cos(p2)*std::cos(std::abs(l1 - l2)))*3437.5;
    printf("The distance to the iceberg: %.2lf miles.\n", d);
    if(d < 99.995)
        printf("DANGER!");
}