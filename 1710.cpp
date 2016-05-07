/* 1710. Boris, You Are Wrong! - http://acm.timus.ru/problem.aspx?num=1710
 *
 * Strategy:
 * The problem essentially asks us to find a triangle that has the same two sides and angle as the
 * given triangle, which we find by projecting the point B on the line AC, and mirroring point C
 * on the other side of point B along the line AC. This new mirrored point must be on the same
 * side of AB as the old point C was. 
 *
 * Performance:
 * Constant, runs the test cases in 0.015s using 260KB memory.
 */

#include <cmath>
#include <iostream>
#include <iomanip>

struct point
{
    double x, y;
    point(double x = 0, double y = 0) : x(x), y(y) { }
    double operator% (const point& p) const { return x*p.y-p.x*y; } // Cross product
    double operator* (const point& p) const { return x*p.x+y*p.y; } // Dot product
    point operator- (const point& p) const { return point(x-p.x, y-p.y); }
    point operator* (double t) const { return point(t*x, t*y); }
    point operator+ (const point& p) const { return point(x+p.x, y+p.y); }
    double dist2() const { return x*x + y*y; }
};

int main()
{
    point A, B, C;
    std::cin >> A.x >> A.y >> B.x >> B.y >> C.x >> C.y;
    point u = B-A, v = C-A;

    point vp = A+v*((u*v)/(v.dist2())); // Vector projection
    point d = C-vp;
    auto C2 = C-d*2;
    if(((C2-B)%(A-B))*((C-B)%(A-B)) < 1e-10 // C2 must lie on the same side of AB as the old C
       || std::abs((C-C2).x) < 1e-10 && std::abs((C-C2).y) < 1e-10) // Can't be the same triangle
    {
        std::cout << "YES";
        return 0;
    }
    std::cout << "NO\n" << std::setprecision(20) << std::fixed;
    std::cout << A.x << " " << A.y << "\n" << B.x << " " << B.y << "\n" << C2.x << " " << C2.y;
}