/* 1633. Hippogriffs - http://acm.timus.ru/problem.aspx?num=1633
 *
 * Strategy:
 * Ternary search. Since it is apparent from the problem description that only the ratio between
 * A and B matters, we can restrict the problem domain to the unit circle. Empirically, it seems
 * like the "influence" function only has one peak over an angle interval of the unit circle, so we
 * can use ternary search to find the maximum value, if we fix the angle interval so that the peak
 * is sure to be somewhere in the middle.
 *
 * Performance:
 * Constant, runs the test cases in 0.015s using 284KB memory.
 */

#include <iomanip>
#include <iostream>
#include <cmath>
#include <limits>

const long double pi = std::acos(-1);

long double a1, b1, a2, b2;

long double f(long double angle) // The square of the influence function
{
    auto B = std::cos(angle), A = std::sin(angle);
    auto Ap = (A*a1 + B*b1), Bp = (A*a2 + B*b2);
    return Ap*Ap+Bp*Bp;
}

int main()
{
    std::cin >> a1 >> b1 >> a2 >> b2;
    // If we have the minimum value near the beginning of the search interval the maximum will be
    // somewhere in the middle
    double min = std::numeric_limits<double>::infinity(), mina;
    for(double a = 0; a < 3.14159; a += 0.1)
        if(f(a) < min)
            min = f(a), mina = a;
    // Ternary search
    long double l = mina, r = mina+pi;
    while(true)
    {
        auto oldr = r, oldl = l;
        auto p1 = l + (r-l)*(1.0/3), p2 = l + (r-l)*(2.0/3);
        auto a = f(p1), b = f(p2);
        if(std::abs(p1-p2) < 1e-17 && std::abs(a-b) < 1e-12)
            break;
        else if(f(p1) > f(p2))
            r = p2;
        else
            l = p1;
        if(r == oldr && l == oldl) // Just to make certain we don't loop endlessly
            break;
    }
    std::cout << std::fixed << std::setprecision(20) << std::sqrt(f((l+r)/2));
}