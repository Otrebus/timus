/* 1948. The Robot on the Line - http://acm.timus.ru/problem.aspx?num=1948
 *
 * Strategy:
 * We solve the equation a(x+i)^2 + b(x+i) + c when i is summed from 1 to k-1, giving
 * kax^2 + (ak(k-1) + bk)x + ak(k-1)(2k-1)/6 + bk(k-1)/2 + ck where we let A, B, C be the 2nd, 1st
 * and 0th degree factor of the quadratic equation as usual. Then we solve for a subzero
 * discriminant B^2 < 4AC, getting k^2 > 1+3*b^2/a^2 - 12*c/a. We also check for k=1 (when there is
 * no summation) by doing the same for the a, b, c of the original equation.
 * 
 * Performance:
 * O(1), runs the tests in 0.015 seconds using 164KB memory.
 */

#include <stdio.h>
#include <cmath>
#include <algorithm>

using ll = long long;
using ld = long double;

ld a, b, c;

ll calc() {
    if(b*b - 4*a*c < 0)
        return 1;
    ld d = 1+3*b*b/(a*a) - 12*c/a;
    return std::ceil(1e-9 + std::sqrt(1+3*b*b/(a*a) - 12*c/a));
}

int main() {
    int t;
    scanf("%d", &t);
    while(t--) {
        scanf("%lf %lf %lf", &a, &b, &c);
        printf("%lld\n", calc());
    }
}
