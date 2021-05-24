/* 1341. Device - http://acm.timus.ru/problem.aspx?num=1341
 *
 * Strategy:
 * Elementary trigonometry. Biggest issue is making sure the longitude is strictly within
 * (-180, 180].
 *
 * Performance:
 * O(1), runs the tests in 0.015s using 296KB memory.
 */

#include <iostream>
#include <cmath>
#include <iomanip>

using ld = long double;
ld pi = 3.14159265359;

ld rd(ld d) { // Rounds the given radians to degrees within (-180, 180]
    ld l = 2*pi;
    ld x = 180*(d/l - std::trunc(d/l))*l/pi; // Truncate to [-pi, pi], turn into degrees
    return x > 180 ? x - 360 : x < -180 ? x + 360 : x; // Still need to truncate!!
}

int main() {
    ld u, v, d, R = 6400;
    std::cin >> v >> u >> d;
    v *= pi/180, u *= pi/180;
    ld v2 = v + d/6400; // Move north
    ld u2 = u + d/(R*std::cos(v2)); // Move east
    ld u3 = u2 - d/(R*std::cos(v)); // Move (south and) west
    std::cout << std::fixed << std::setprecision(20) << 180*v/pi << " " << rd(u3);
}
