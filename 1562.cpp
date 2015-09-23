/* 1562. GM-pineapple - http://acm.timus.ru/problem.aspx?num=1562
 *
 * Strategy:
 * Calculus.
 *
 * Performance:
 * O(n), runs in 0.015 and uses 268KB memory.
 */

#include <cmath>
#include <stdio.h>

const double pi = 3.141592653589793238462643383279502884;

double integrate(double w, double h, double t)
{
    return (pi*h*h/4.0)*(2.0*t*t/w-4.0*t*t*t/w/w/3.0);
}

int main()
{
    double w, h, n;
    scanf("%lf %lf %lf", &h, &w, &n);
    for(int i = 0; i < n; i++)
        printf("%lf\n", integrate(w, h, (i+1)*w/n) -
                        integrate(w, h, i*w/n));
    return 0;
}