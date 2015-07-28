/* 1828. Approximation by a Progression - http://acm.timus.ru/problem.aspx?num=1828
 *
 * Strategy:
 * Simple regression.
 *
 * Performance:
 * O(n). Runs the test cases in 0.001s using 300Kb memory.
 */

#include <stdio.h>
#include <vector>

int main()
{
    long long n;
    scanf("%lld", &n);
    std::vector<int> y(n);
    for(int i = 0; i < n; i++)
        scanf("%d", &(y[i]));
    double xyb = 0;
    for(int i = 0; i < n; i++)
        xyb += i*y[i];
    xyb /= n;
    double yb = 0;
    for(int i = 0; i < n; i++)
        yb += y[i];
    yb /= n;
    double xb = (n*(n-1))/2/double(n);
    double xb2 = ((n-1)*n*(2*(n-1)+1))/6/double(n);
    double b = (xyb - xb*yb)/(xb2 - xb*xb);
    double a = yb - b*xb;
    printf("%lf %lf\n", a, b);
    return 0;
}