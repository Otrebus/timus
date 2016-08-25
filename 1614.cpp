/* 1614. National Project "Trams" - http://acm.timus.ru/problem.aspx?num=1614
 *
 * Strategy:
 * Brute-forcing random solutions for low N's gives a few interesting patterns which seem to
 * generalize. The below program generates one such pattern.
 *
 * Performance:
 * O(n^2), runs the tests in 0.031s using 260KB memory.
 */

#include <iostream>

int main()
{
    int n;
    std::cin >> n;
    for(int i = 0; i < n; i++)
    {
        for(int j = 0, c = i; j < 2*n; j++)
            std::cout << (1 + (c += j)%(2*n)) << " ";
        std::cout << "\n";
    }
}