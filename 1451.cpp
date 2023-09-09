/* 1451. Beerhouse Tale - http://acm.timus.ru/problem.aspx?num=1451
 *
 * Strategy:
 * Along any line the function we want to minimize is bimodal, so we run a ternary search along
 * some random line to find the minimal point of f along that line, at that point check roughly
 * which direction the function has a maximal gradient, then run ternary search along that line,
 * and keep repeating.
 *
 * Performance:
 * Hard to derive the complexity, runs the tests in 0.109s using 300KB memory.
 */

#include <iostream>
#include <cmath>
#include <iomanip>

using ld = long double;
const ld pi = 3.14159265;

struct vec {
    ld x, y;
    vec operator+ (const vec& v) const { return { x+v.x, y+v.y }; };
    vec operator- (const vec& v) const { return { x-v.x, y-v.y }; };
    vec operator* (ld t) const { return { t*x, t*y }; };
    vec operator/ (ld t) const { return { x/t, y/t }; };
    ld length() { return std::sqrt(x*x + y*y); }
};

vec p1, p2, p3;

double f(vec s) { // Function we want to minimize
    return (s-p1).length() + (s-p2).length() + (s-p3).length();
}

vec ternary(vec s, vec dir) {
    // Ternary search of f along the line that passes through s in the direction dir
    vec L = s + dir*100;
    vec R = s - dir*100;
    while((R - L).length() > 1e-8)
    {
        ld x = f(L + (R - L)/3);
        ld y = f(L + (R - L)*2.0L/3);
        if(x < y)
            R = L + (R - L)*2.0L/3;
        else
            L = L + (R - L)/3;
    }
    return R;
}

int main() {
    std::cin >> p1.x >> p1.y >> p2.x >> p2.y >> p3.x >> p3.y;

    vec s = (p1 + p2 + p3)/3; // Starting point of our ternary search
    for(int i = 0; i < 10000; i++) {
        ld minF = 10000000;
        vec minDir = { 0, 0 };
        for(int i = 0; i < 200; i++) { // Find the line with max gradient
            vec dir = { 1e-6*std::cos(i*pi/100), 1e-6*std::sin(i*pi/100) };
            ld val = f(s + dir);
            if(val < minF) {
                minDir = dir*1e6;
                minF = val;
            }
        }
        s = ternary(s, minDir); // Ternary search along the above found line
    }
    std::cout << std::setprecision(20) << s.x << " " << s.y;
}
