/* 1961. Cantonese Dialect - http://acm.timus.ru/problem.aspx?num=1961
 *
 * Strategy:
 * Solve (M/N)*n = m for M and round up.
 *
 * Performance:
 * Constant, runs in 0.015s using 260KB memory.
 */

#include <iostream>
#include <algorithm>

int main()
{
    long long n, m, N;
    std::cin >> n >> m >> N;
    std::cout << std::min((N+1)*m/n, N);
}