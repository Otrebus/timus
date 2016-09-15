/* 1988. Planet Ocean Landing - http://acm.timus.ru/problem.aspx?num=1988
 *
 * Strategy:
 * Translate the problem onto a 2d surface by using the angle between the location vectors of the
 * spaceship and the carrier (with the planet center being the origin) and placing the spaceship
 * on the x axis. Then, figure out the tangent point of the planet with respect to the spaceship.
 * This allows us to easily calculate the total travelling time if the spaceship and carrier meets
 * up at some point on the surface given by the angle between the point and the x axis. Ternary
 * search can then be used to calculate the optimal time.
 *
 * Performance:
 * Constant, runs the tests in 0.001s using 336KB memory.
 */

#include <iomanip>
#include <cmath>
#include <iostream>
#include <algorithm>

struct point
{
    double x, y;
    point(double x = 0, double y = 0) : x(x), y(y) {}
    point operator- (const point& p) { return point(x - p.x, y - p.y); }
    point operator/ (const double t) { return point(x/t, y/t); }
    double dist() { return std::sqrt(x*x + y*y); }
};

int main()
{
    double x1, y1, z1, x2, y2, z2, v;
    std::cin >> x1 >> y1 >> z1 >> x2 >> y2 >> z2 >> v;
    double r = std::sqrt(x1*x1 + y1*y1 + z1*z1); // Distance to the carrier
    double d = std::sqrt(x2*x2 + y2*y2 + z2*z2); // Distance to the spaceship
    double t = std::acos((x1*x2 + y1*y2 + z1*z2)/(r*d)); // Angle between them
    
    const double pi = std::acos(-1.0);
    double D = std::sqrt(d*d - r*r); // Distance to the tangent point
    double s = pi/2 - atan(r/D); // Angle between the x axis (on which the ship is) and above

    // Calulates the greatest of the time taken to transport the carrier and the spaceship
    // to the point on the planet with angle u with the x axis
    auto calc = [&t, &s, &r, &v, &d, &D] (double u)
    {
        point p(std::cos(u)*r, std::sin(u)*r), B(d, 0); // Meeting point and spaceship location
        return u < s ? std::max((t - u)*r, (p - B).dist()/v) // Meeting point visible by spaceship
                     : std::max(D/v + (u - s)*r/v, (t - u)*r); // Meeting point beyond tangent point
    };

    double L = 0, R = t, x, y;
    for(int i = 0; i < 50; i++) // Ternary search
    {
        x = calc(L + (R - L)/3);
        y = calc(L + 2*(R - L)/3);
        if(x < y)
            R = L + 2*(R - L)/3;
        else
            L = L + (R - L)/3;
    }
    std::cout << std::setprecision(20) << x;
}