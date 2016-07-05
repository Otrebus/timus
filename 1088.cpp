/* 1088. Ilya Murometz - http://acm.timus.ru/problem.aspx?num=1088
 *
 * Strategy:
 * Naive LCA.
 *
 * Performance:
 * O(F^2), runs in 0.015s using 236KB memory.
 */

#include <iostream>

int main()
{
    int D, E, F, Dp, Ep, H;
    std::cin >> D >> E >> F >> Dp >> Ep >> H;
    Dp = (Dp + (1 << F) - 1) >> D;
    Ep = (Ep + (1 << F) - 1) >> E;
    for(int y = 0, Q = 0; !Q; Ep /= 2, ++y)
        for(int p = Dp, x = 0; p && !Q; p /= 2, x++)
            if(Q = (p == Ep))
                std::cout << (x + y <= H ? "YES" : "NO");
}