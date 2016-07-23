/* 2008. Swifty - http://acm.timus.ru/problem.aspx?num=2008
 *
 * Strategy:
 * Ballistics. See eg https://en.wikipedia.org/wiki/Trajectory_of_a_projectile.
 *
 * Performance:
 * Constant, runs in 0.015s using 240KB memory.
 */

#include <iostream>

bool f(long long v, long long x1, long long y1, long long x2, long long y2)
{
    long long x = x2-x1, y = y2-y1;
    if(v == 0)
        return x == 0 && y <= 0;
    return 100*v*v*v*v >= 98*98*x*x+2*10*98*y*v*v;
}

int main()
{
    long long x1, y1, x2, y2, x3, y3, v;
    std::cin >> v >> x1 >> y1 >> x2 >> y2 >> x3 >> y3;
    if(f(v, x2, y2, x3, y3) && f(v, x3, y3, x2, y2) 
       && (f(v, x3, y3, x1, y1) || f(v, x2, y2, x1, y1)))
        std::cout << "Yes";
    else
        std::cout << "No";
}