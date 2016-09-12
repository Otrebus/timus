/* 1647. Divide an Island! - http://acm.timus.ru/problem.aspx?num=1647
 *
 * Strategy:
 * Express the two points that we are looking for as the parameters s, t in [0, 1] in the parametric
 * equation for two lines in the triangle. If the sides of the triangle have length A, B and C and
 * the angle between A and B is u, the area constraint gives us sAtBcos u = 0.5ABcos u, so
 * t = 0.5/s, and the perimeter constraint gives us sA + tB + C = 0.5(A + B + C). This system of
 * equations gives us a second degree equation in t that we can solve and check for consistency for
 * the three different assignments of sides A, B and C.
 *
 * Performance:
 * Constant, runs the tests in 0.015s using 316KB memory.
 */

#include <iomanip>
#include <cmath>
#include <iostream>

struct point
{
    double x, y;
    point(double x = 0, double y = 0) : x(x), y(y) {}
    point operator- (const point& p) { return point(x - p.x, y - p.y); }
    point operator+ (const point& p) { return point(x + p.x, y + p.y); }
    point operator* (const double t) { return point(x*t, y*t); }
    double dist() { return std::sqrt(x*x + y*y); }
};

bool solve(point p1, point p2, point p3, point& S, point& T)
{
    double a = (p2-p1).dist(), b = (p3-p1).dist(), c = (p3-p2).dist();
    double p = a + b + c;
    double t;
    if(std::abs(p*p - 8*a*b) < 1e-9)
        t = p/(4*b);
    else
    {
        t = (p + std::sqrt(p*p - 8*a*b))/(4*b);
        if(t > 1)
            t = (p - std::sqrt(p*p - 8*a*b))/(4*b);
    }
    double s = 1/(2*t);
    if(t != t || s > 1 + 1e-9 || s < 0 - 1e-9 || t > 1 + 1e-9 || t < 0 - 1e-9)
        return false;
    S = p1 + (p2-p1)*s;
    T = p1 + (p3-p1)*t;
    return true;
}

int main()
{
    point p1, p2, p3, S, T;
    std::cin >> p1.x >> p1.y >> p2.x >> p2.y >> p3.x >> p3.y;

    while(!solve(p1, p2, p3, S, T))
        std::swap(p1, p2), std::swap(p2, p3);
    std::cout << "YES\n" << std::setprecision(20) << S.x << " " << S.y << "\n" << T.x << " " << T.y;
}