/* 1373. Pictura ex Machina - http://acm.timus.ru/problem.aspx?num=1373
 *
 * Strategy:
 * Use elementary vector arithmetic to calculate the turning point between any given input points
 * and calculate the bounding height and width of all resulting points.
 *
 * Performance:
 * O(N), runs the tests in 0.359s using 304KB memory.
 */

#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iomanip>

struct point {
    double x, y;

    point(double a, double b) : x(a), y(b) {}
    point operator+(const point& p) const { return { x + p.x, y + p.y }; }
    point operator-(const point& p) const { return { x - p.x, y - p.y }; }
    point operator/(double p) const { return { x/p, y/p }; }
};

point turn(const point& a, const point& b) {
    point p = b - a;
    point pp = point(-p.y, p.x); // Rotate the line segment 90 degrees left
    point pd = b - a - pp;
    // The turning point is the halfway point between the rotated segment and the original
    return a + pp + pd/2;
}


int main() {

    std::string line;
    double minx = 10001, maxx = -10001, miny = 10001, maxy = -10001;

    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    while (std::getline(std::cin, line))
    {
        std::stringstream ss(line);
        int a, b, c, d;
        ss >> a >> b >> c >> d;
        point p1(a, b), p2(c, d), p3 = turn(p1, p2);
        
        minx = std::min( { p1.x, p2.x, p3.x, minx } );
        maxx = std::max( { p1.x, p2.x, p3.x, maxx } );

        miny = std::min( { p1.y, p2.y, p3.y, miny } );
        maxy = std::max( { p1.y, p2.y, p3.y, maxy } );
    }

    std::cout << std::fixed << std::setprecision(4)
              << std::max(maxx - minx, 0.0) << " " << std::max(maxy - miny, 0.0) << std::endl;
}
