/* 1232. Asteroid Landing - http://acm.timus.ru/problem.aspx?num=1232
 *
 * Strategy:
 * If the remaining vertical distance isn't divisible by the segment length, angle the next
 * segment towards the side to make its vertical component equal to the remainder (and at most
 * towards the edge of the beam). If the remaining distance is divisible by the segment length,
 * the solution is trivial (drop straight down). Repeat until the bottom is reached.
 *
 * Performance:
 * O(n), runs the tests in 0.015s using an unreported amount of memory.
 */

#include <vector>
#include <stdio.h>
#include <cmath>

using ld = long double;
const ld eps = 1e-6;
const ld pi = std::acos(-1);
ld h, d, a;

struct point { ld x, y; };

void fill(point p, std::vector<point>& out) {
    // Assuming the altitude is divisible by d, this adds segments vertically to the bottom
    while(p.y > eps) {
        p.y -= d;
        out.push_back(p);
    }
}

int main() {
    std::vector<point> out;

    scanf("%lf %lf %lf", &h, &d, &a);

    a /= 2;
    ld r = d*((h/d) - std::floor(h/d)); // The remainder
    ld t = r > eps ? std::acos(r/d) : 0; // The angle needed to "lift" the remainder

    // The first point of the segment
    point p = { -d*std::sin(std::min(t, a)), h - std::cos(std::min(t, a))*d };
    out.push_back(p);
    if(t < a) { // If the angle needed is inside the beam, we are done
        fill(p, out);
    } else { // Otherwise, we zig-zag inside the beam
        int side = 1;

        while(true) {
            ld t2 = a + pi/2;
            ld w = (h-p.y)*std::tan(a)+std::abs(p.x); // Horizontal distance to the side of the beam
            // The angle from the current position to the side of the beam:
            ld maxt = pi/2 - (w < d ? (pi - std::asin(std::sin(t2)*w/d) - t2) : 0);

            // How much we need to turn, like the above
            ld r = eps + d*((p.y/d) - std::floor(p.y/d));
            ld t = r > eps ? std::acos(r/d) : 0;

            // Turn how much we actually can
            p.y -= d*std::cos(std::min(t, maxt));
            p.x += side*d*std::sin(std::min(t, maxt));
            out.push_back(p);
            if(t < maxt) { // If we didn't turn the maximum amount, means we can just drop
                fill(p, out);
                break;
            }

            t = maxt;
            side *= -1; // Move towards the other side
        }
    }

    for(auto& p : out) // If we end up below the surface, there is necessarily no answer
        if(p.y < -eps*2)
            return printf("-1"), 0;

    printf("%d\n", out.size()); // Output the segments
    for(auto& p : out)
        printf("%lf 0 %lf\n", p.x, p.y);
}
