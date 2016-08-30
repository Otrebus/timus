/* 2098. Lada Priora - http://acm.timus.ru/problem.aspx?num=2098
 *
 * Strategy:
 * Straightforward computation.
 *
 * Performance:
 * Constant, runs the tests in 0.001s using 312KB memory.
 */

#include <iostream>
#include <iomanip>
#include <cmath>

int main()
{
    long long b, c, d, k, p;
    std::cin >> b >> c >> d >> k >> p;
    double dif = std::abs((k*c + d*k*(k + 1)/2)*(1 + double(p)/100) - (b + k*c + d*k*(k + 1)/2));
    if(k*c*p + d*p*k*(k + 1)/2 > 100*b)
        std::cout << "Cash\n" << std::fixed << std::setprecision(10) << dif;
    else
        std::cout << "Insurance\n" << std::fixed << std::setprecision(10) << dif;
}