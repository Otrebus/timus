/* 1775. Space Bowling - http://acm.timus.ru/problem.aspx?num=1775
 *
 * Strategy:
 * For each pair of points, sort the other points by distance from the line spanning the pair and
 * use the kth such distance to update the global minimum.
 *
 * Performance:
 * O(N^3 log N), runs the tests in 0.764s using 404KB memory.
 */

#include <vector>
#include <algorithm>
#include <iostream>
#include <iomanip>

struct point
{
    long long x, y;
    point operator-(const point& b) { return { x-b.x, y-b.y }; }
    long long dist2() { return x*x + y*y; }
} points[200];

int main()
{
    double ans = std::numeric_limits<double>::infinity();
    int n, k;
    std::cin >> n >> k;
    for(int i = 0; i < n; i++)
        std::cin >> points[i].x >> points[i].y;
    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < n; j++)
        {
            std::vector<double> v = { 0, 0 };
            for(int k = 0; k < n; k++)
            {
                if(k == i || k == j)
                    continue;
                point p1 = points[i], p2 = points[j], p0 = points[k];
                point u = p2 - p1;
                point w = p0 - p1;
                if(u.x*w.y - u.y*w.x > 0) // On the wrong side
                    continue;
                // Calculate the distance from the point to the line
                double n = (p2.y - p1.y)*p0.x - (p2.x - p1.x)*p0.y + p2.x*p1.y - p2.y*p1.x;
                v.push_back(std::sqrt(n*n/double(u.dist2())));
            }
            std::sort(v.begin(), v.end() );
            if(v.size() > k-1 && ans > v[k-1])
                ans = v[k-1];
        }
    }
    if(ans == std::numeric_limits<double>::infinity() || std::abs(ans-0) < 1e-9)
        std::cout << "0.000000";
    else
        std::cout << std::fixed << std::setprecision(10) << std::max(0.0, ans - 1.0);
}