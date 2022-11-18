/* 1442. Floo Powder - http://acm.timus.ru/problem.aspx?num=1442
 *
 * Strategy:
 * The negative space formed in the cone is equivalent to two cones with a parabolic base and an
 * apex at that of the original cone. Their volumes turn out to be equal since the width of the
 * base of one equals the height of the other, and vice versa.
 *
 * Performance:
 * Constant, runs the tests in 0.015s using 192KB memory.
 */

#include <stdio.h>
#include <cmath>

const auto pi = std::acos(-1.0);

// Calculates the volume of a cone with radius b cut from a larger cone
double calc(double r, double d, double h, double b) {
    auto L = std::sqrt(h*h+r*r); // The length of the side of the large cone
    auto cost = r/L;

    auto H = b/2/cost; // The length of the side of the small cone
    auto a = std::sqrt(r*r-d*d); // Half the base of the parabola

    auto A = 4*H*a/3; // Area of the parabola

    auto p = std::asin(b/(2*H));
    auto j = (L-H)*std::cos(pi/2 - 2*p); // Non-skewed height of the small cone

    return j*A/3;
}

int main() {
    double r, d, h;
    scanf("%lf %lf %lf", &r, &d, &h);

    auto v = calc(r, d, h, r+d);

    auto ans = r*r*pi*h/3 - (d < r ? 2*v : 0);

    printf("%.10lf", ans);
}
