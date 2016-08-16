/* 1405. Goat in the Garden 5 - http://acm.timus.ru/problem.aspx?num=1405
 *
 * Strategy:
 * Straightforward calculation according to the rules given - note that the maximum area of two
 * abutting expandable circles always occurs when one of the circles is at its maximum radius.
 *
 * Performance:
 * O(N^2), runs the tests in 0.031s using 300KB memory.
 */

#include <iomanip>
#include <iostream>
#include <cmath>
#include <algorithm>

const int maxn = 1000;
const double maxrange = 50, minrange = 1;

struct point
{
    double x, y;
    point(double x = 0, double y = 0) : x(x), y(y) { }
    point operator-(const point& p) { return point(x-p.x, y-p.y); }
    double dist() { return std::sqrt(x*x + y*y); }
} points[maxn];

double range[maxn]; // Maximum range of a goat tethered to this tree

int main()
{
    int N;
    double ans = 0, pi = std::acos(-1);
    std::cin >> N;
    for(int i = 0; i < N; i++)
        range[i] = maxrange, std::cin >> points[i].x >> points[i].y;
    // Clamp the maximum range of each tree to how close the closest other tree is
    for(int i = 0; i < N; i++)
        for(int j = 0; j < N; j++)
            if(i != j)
                range[i] = std::min(range[i], (points[i] - points[j]).dist());
    for(int i = 0; i < N; i++)
    {
        for(int j = i + 1; j < N; j++)
        {
            double d = (points[j]-points[i]).dist();
            if(d < 2*minrange)
                continue;
            else if(range[i] >= minrange && range[j] >= minrange)
            {
                // Extend the rope of goat j far as possible
                double a = std::min(range[i], std::max(minrange, d - range[j]));
                double b = std::min(range[j], d - a);
                ans = std::max(ans, a*a*pi + b*b*pi);
                // Extend the other rope as far as possible
                a = std::min(range[j], std::max(minrange, d - range[i]));
                b = std::min(range[i], d - a);
                ans = std::max(ans, a*a*pi + b*b*pi);
            }
        }
    }
    std::cout << std::fixed << std::setprecision(4) << ans;
}