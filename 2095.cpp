/* 2095. Scrum - http://acm.timus.ru/problem.aspx?num=2095
 *
 * Strategy:
 * Repeated division.
 *
 * Performance:
 * O(sqrt(r)), runs the tests in 0.001s using 308KB memory.
 */

#include <iostream>

int main()
{
    int l, r;
    std::cin >> l >> r;
    auto f = [] (int n) { for(int i = 2; n >= i; n -= n/i++); return n; };
    std::cout << f(r) - f(l-1);
}