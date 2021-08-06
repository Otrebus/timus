/* 1360. Philosophical Dispute - http://acm.timus.ru/problem.aspx?num=1360
 *
 * Strategy:
 * Set t to acos(y) to get the y coordinate right, increase t by 2pi until x meets the precision.
 *
 * Performance:
 * O(1/ε), runs the tests in 0.015s using 16KB memory.
 */

#include <iostream>
#include <cmath>
#include <iomanip>

int main() {
    long double x, y, eps;
    std::cin >> x >> y >> eps;

    for(long double t = std::acos(y); ;t += 6.28318530718)
        if(std::abs(std::sin(std::sqrt(t))-x) < eps)
            return std::cout << std::fixed << std::setprecision(10) << t, 0;
}
