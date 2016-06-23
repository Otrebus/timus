/* 1234. Bricks - http://acm.timus.ru/problem.aspx?num=1234
 *
 * Strategy:
 * Geometry.
 *
 * Performance:
 * Constant, runs in 0.015s using 248KB memory.
 */

#include <algorithm>
#include <iostream>

bool fit(long long a, long long b, long long c, long long d)
{
    auto A = 2*a*b*c, B = a*a - b*b, C = a*a + b*b - c*c, D = a*a + b*b;
    if(a < b || c < d || (a == b) && (c == d) && a > c)
        return false;
    return a <= c && b <= d || A <= d*D && a > c && d*d*D*D - 2*A*d*D + A*A >= B*B*C;
}

int main()
{
    int A[5];
    for(int i = 0; i < 5; i++)
    {
        double d;
        std::cin >> d;
        A[i] = (d+0.01)*10;
    }
    std::sort(A, A+3);
    bool b = false;
    do
    {
        b |= fit(A[0], A[1], A[3], A[4]) || fit(A[0], A[1], A[4], A[3]);
    } while(std::next_permutation(A, A+3));
    printf(b ? "YES" : "NO");
}