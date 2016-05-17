/* 1059. Expression - http://acm.timus.ru/problem.aspx?num=1059
 *
 * Strategy:
 * Horner's rule.
 *
 * Performance:
 * O(n), runs in 0.001s using 260KB memory.
 */

#include <iostream>

int main()
{
    int n;
    std::cin >> n;

    std::cout << "0\n";
    for(int i = 1; i <= n; i++)
        std::cout << "X\n*\n" << i << "\n+\n";
}