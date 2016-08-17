/* 1484. Film Rating - http://acm.timus.ru/problem.aspx?num=1484
 *
 * Strategy:
 * First, find the largest sum of the votes s that gives rise to the given x by solving
 * x < s/N - 0.05 and clamping this to n*10. Then find the smallest number t of extra "one" ratings
 * that we add to this to produce the average y, which is given by solving the equation for the
 * average (and rounding down), (s + t)/(N + t) < y + 0.05, for t. In the given program, for
 * numerical accuracy, the numbers x and y are integers representing the original number multiplied
 * by 10.
 *
 * Performance:
 * Constant, runs the tests in 0.015s using 260KB memory.
 */

#include <iostream>
#include <algorithm>

int main()
{
    double fx, fy;
    int n;
    std::cin >> fx >> fy >> n;
    int x = fx*10+0.01, y = fy*10+0.01;
    std::cout << (x <= y ? 0 : (10*n*y+5*n-100*std::min(10*n, (n*(10*x+5)-1)/100))/(100-10*y-5)+1);
}