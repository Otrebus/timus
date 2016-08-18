/* 1825. Ifrit Bomber 2 - http://acm.timus.ru/problem.aspx?num=1825
 *
 * Strategy:
 * Calculate the area of the two annuli, and then subtract the part that intersects. The latter part
 * can either be arrived to through application of the inclusion-exclusion principle or as breaking
 * down the intersection as 
 * 
 *   A1 i A2 = A1 i (C2 - c2) 
 *           = C1 i (C2 - c2) - c1 i (C2 - c2)
 *           = C1 i C2 - C1 i c2 - c1 i C2 + c1 i c2, 
 *
 * where A1 and A2 are the annuli, C1 and c1 is the major and minor circles of A1 and i is the
 * intersection operator.
 *
 * Performance:
 * Constant, runs the tests in 0.001s using 280KB memory.
 */

#include <iomanip>
#include <iostream>
#include <cmath>

const double pi = std::acos(-1);

// Calculates the area of the intersection of two circles at distance d with radii r1 and r2
double area(double r1, double r2, double d)
{
    if(d > r1 + r2)
        return 0;
    else if (d <= std::abs(r2 - r1))
        return pi*(r1 >= r2 ? r2*r2 : r1*r1);
    double p = 2.0*std::acos((r1*r1 + d*d - r2*r2)/(2.0*r1*d));
    double t = 2.0*std::acos((r2*r2 + d*d - r1*r1)/(2.0*r2*d));
    return 0.5*(r2*r2*(t - std::sin(t)) + r1*r1*(p - std::sin(p)));
}

int main()
{
    double d, r1, R1, r2, R2;
    std::cin >> d >> r1 >> R1 >> r2 >> R2;
    double ans = pi*(R1*R1+R2*R2-r1*r1-r2*r2) -
                 (area(R1, R2, d) - area(R1, r2, d) - area(r1, R2, d) + area(r1, r2, d));
    std::cout << std::fixed << std::setprecision(20) << ans;
    std::system("pause");
}