/* 1428. Jedi Riddle - http://acm.timus.ru/problem.aspx?num=1428
 *
 * Strategy:
 * Let Z = 2 and C-1 = nA = mB. Then, X^A + Y^B = X^((C-1)/n) + Y^((C-1)/m = 2^C. If we let
 * X^((C-1)/n) = 2^(C-1) we see that X = 2^((C-1)/A) is a solution, and similar for Y.
 *
 * Performance:
 * 0.015s, 384KB.
 */

#include <iostream>

unsigned long long pw(unsigned long long x, unsigned int y)
{
    unsigned long long r = 1;
    while(y--)
        r *= x;
    return r;
}

int main()
{
    int A, B, C;
    std::cin >> A >> B >> C;
    std::cout << pw(2, (C-1)/A) << "\n" << pw(2, (C-1)/B) << "\n" << 2 << "\n";
}