/* 1550. Dean's Pyramid 3 - http://acm.timus.ru/problem.aspx?num=1550
 *
 * Strategy:
 * Elementary geometry and volume formulas.
 *
 * Performance:
 * Constant complexity, runs the test cases in 0.015s using 272KB memory.
 */

#include <algorithm>
#include <iostream>
#include <iomanip>

int main()
{
    double H, W, X, Y, R, pi = std::acos(-1);
    std::cin >> H >> W >> X >> Y >> R;
    double h = (2*H/W)*(W/2-std::max(std::abs(X), std::abs(Y)));
    std::cout << std::fixed << std::setprecision(5) << (W*W*H*(1.0/3.0) - (R*R*pi*h));
}
