/* 1729. Dead Man's Chest - http://acm.timus.ru/problem.aspx?num=1729
 *
 * Strategy:
 * Imagine that the island is a pie divided into slices of A degrees plus one remainder slice. The
 * optimal solution is then to walk from the middle of the circle to the junction between two
 * slices on the edge of the circle (that way we visit them both within an infinitesimal movement).
 * Then, we keep going to the next junction between the next two unvisited slices, until we reach
 * the remainder slice, along with a possible odd slice, which we handle separately.
 *
 * Performance:
 * Constant, runs the tests in 0.015s using 456KB memory.
 */

#include <iostream>
#include <cmath>

const long double pi = std::acos(-1);

long double d(long double r, long double a) // Law of sines for symmetric side angles
{
    a *= pi/180000.0;
    return r*std::sin(a)/(std::sin((pi-a)/2.0));
}

int main()
{
    long double r, A;
    std::cin >> r >> A;
    int a = (int) (1000.0*(A+0.0001)); // For more exact calculations
    long double ans = r;
    int n = (360000-2*a)/(2*a); // Number of slice pairs apart from the first we reach
    ans += n*d(r, 2*a); // Visit each pair junction
    ans += d(r, 360000%a+((360000/a)%2 == 1 ? a : 0)); // Odd slice + remainder slice
    std::cout.precision(7);
    std::cout << std::fixed << ans;
}