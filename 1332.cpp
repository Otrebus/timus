/* 1332. Genie Bomber - http://acm.timus.ru/problem.aspx?num=1332
 *
 * Strategy:
 * First, translate the problem by reducing the explosion radius by the radius of the cities and
 * making the cities point sized. Then, for reach pair of point-sized cities, place the genie in
 * the two different ways such that its explosion exactly touches those two points and calculate
 * how many cities in total are obliterated, and use the maximum result.
 *
 * Performance:
 * O(N^3), runs in 0.031s using 272KB memory.
 */

#include <algorithm>
#include <iostream>

struct point
{
    double x, y;
    point(double x = 0, double y = 0) : x(x), y(y) { }
    point operator- (const point& p) const { return point(x-p.x, y-p.y); }
    point operator* (double t) const { return point(t*x, t*y); }
    point operator/ (double t) const { return point(x/t, y/t); }
    point operator+ (const point& p) const { return point(x+p.x, y+p.y); }
    point normalized() const { return point(x/dist(), y/dist()); }
    double dist2() const { return x*x + y*y; }
    double dist() const { return std::sqrt(x*x + y*y); }
} points[100];

const double eps = 1e-6;

int main()
{
    int N, x, y, R, r;
    std::cin >> N;
    for(int i = 0; i < N; i++)
        std::cin >> points[i].x >> points[i].y;
    std::cin >> R >> r;
    R -= r;
    int max = R >= 0;
    for(int i = 0; i < N; i++)
    {
        for(int j = i+1; j < N; j++)
        {
            int A = 0, B = 0;
            // Figure out where the two centers of the two circles of radius R that intersect
            // points i and j are
            point m = (points[j]+points[i])/2; // Midpoint of i and j
            point v = points[j]-points[i]; // The vector between the points
            point perp = point(-v.y, v.x); // Vector perpendicular the above
            double l = std::sqrt(R*R-v.dist2()/4); // The length of the leg along perp
            // If the cities are more than 2R apart, l becomes NaN and everything after this
            // becomes false, so max won't be updated. Slower execution but smaller code
            point a = m + perp.normalized()*l, b = m - perp.normalized()*l;
             
            for(int k = 0; k < N; k++)
            {
                if((points[k]-a).dist() <= R + eps)
                    A++;
                if((points[k]-b).dist() <= R + eps)
                    B++;
            }
            max = std::max( { max, A, B } );
        }
    }
    std::cout << max;
}