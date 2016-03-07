/* 1605. Devil's Sequence - http://acm.timus.ru/problem.aspx?num=1605
 *
 * Strategy:
 * By using the method of generating functions or some computer algebra program we find that the
 * closed form equation of the series is x_n = 2/3 + 1/3*(-1/2)^(n-1) = 0.666.. + y. When y is
 * positive, we find the leftmost '6', at digit k, that is flipped by y by solving the equation
 * y = 1/3*(-1/2)^(n-1) >= 1/3*10^-k, which is done below. The case for when y is negative is solved
 * similarly; the two solutions are merged into the same equation.
 *
 * Performance:
 * O(1), runs the test cases in 0.001s using 0.001kB memory.
 */

#include <stdio.h>
#include <cmath>

int main()
{
    int n;
    scanf("%d", &n);
    printf("%d\n", int(-n/2*2*std::log(0.5)/std::log(10)));
}