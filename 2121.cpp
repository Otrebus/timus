/* 2121. Intersection of Parabolas - http://acm.timus.ru/problem.aspx?num=2121
 *
 * Strategy:
 * Integrate the given equations, but instead of trying to simplify the resulting monster expression
 * manually do so the programmer way and write a program to find a pattern from the different values
 * of a.
 *
 * Performance:
 * O(1), runs the tests in 0.015s using 260KB memory.
 */

#include <iostream>

int main() {
    long long a;
    std::cin >> a;
    std::cout << (4*a - 1) << ".6666666667";
}

// Program used to find the pattern:
/*
#include <iostream>
#include <cmath>
#include <ios>

typedef long double ld;

ld a;

ld f1(ld x) {
    return x*x*x/3.0 - a*x*x + a*a*x;
}

ld f2(ld x) {
    return a*x + 2.0l*std::pow(x, 1.5l)/3.0l;
}

ld f3(ld x) {
    return a*x - 2.0l*std::pow(x, 1.5l)/3.0l;
}

int main() {
    std::cin >> a;
    ld x1 = (2.0l*a - 1.0l - std::sqrt(4.0l*a - 3.0l))/2.0l;
    ld x2 = (2.0l*a + 1.0l - std::sqrt(4.0l*a + 1.0l))/2.0l;
    ld x3 = (2.0l*a - 1.0l + std::sqrt(4.0l*a - 3.0l))/2.0l;
    ld x4 = (2.0l*a + 1.0l + std::sqrt(4.0l*a + 1.0l))/2.0l;

    ld ans = f2(x4) - f2(x1) - (f1(x4) - f1(x3) + f3(x3) - f3(x2) + f1(x2) - f1(x1));
    std::cout.precision(10);
    std::cout << std::fixed << ans;
}
*/
