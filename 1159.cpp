/* 1159. Fence - http://acm.timus.ru/problem.aspx?num=1159
 *
 * Strategy:
 * The polygon with the maximum area must be cyclic, ie. all points must lie on the perimeter of
 * some circle. What remains is to figure out the radius of that circle, which is done via binary
 * search.
 *
 * Performance:
 * O(N log N), runs the test cases in 0.001s using 300KB memory.
 */

#include <numeric>
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <vector>

const double pi = std::acos(-1);

struct point
{ 
    double x, y; 
    point(double x = 0, double y = 0) : x(x), y(y) {}
    point operator-(point p) { return point(x - p.x, y - p.y); }
    double dist() { return std::sqrt(x*x + y*y); }
};

// Calculates the area of a polygon of edges of lengths of all but the last integer in the given
// vector whose vertices all lie along a circle with radius r, and also returns the difference
// between the length of the last edge and the side that would complete of the polygon
std::pair<double, double> calc(std::vector<int>& v, double r)
{
    double area = 0, sum = 0;
    std::vector<point> ps = { { r, 0 } };
    for(int i = 0; i < v.size() - 1; i++)
    {
        double L = v[i];
        double p = std::acos((2*r*r-L*L)/(2*r*r)); // The angle subtended by this edge
        sum += p;
        if(sum >= 2*pi) // We wrap around, counts as giving too little space for the last edge
            return { -1, 0 };
        ps.push_back( { r*std::cos(sum), r*std::sin(sum) } );
    }
    for(int i = 0; i < ps.size(); i++)
        area += ps[i].x*ps[(i+1)%ps.size()].y - ps[(i+1)%ps.size()].x*ps[i].y;
    return { (point(r, 0)-ps.back()).dist()-v.back(), area/2 };
}

int main()
{
    int N;
    std::cin >> N;
    std::vector<int> v;
    while(N--)
    {
        int x;
        std::cin >> x;
        v.push_back(x);
    }
    // We sort to make sure the edge that spans back through the circle is the biggest one
    std::sort(v.begin(), v.end());
    double l = 0, r = 1e6, sum, area;

    // Binary search over the radius of the circle
    for(int i = 64; i; i--)
    {
        double mid = l + (r-l)/2;
        auto p = calc(v, mid);
        sum = p.first, area = p.second;
        if(v.back() > 2*mid || sum < 0)
            l = mid;
        else
            r = mid;
    }
    if(v.back() >= std::accumulate(v.begin(), v.end()-1, 0)) // Degenerate or impossible polygon
        std::cout << "0.00";
    else
        std::cout << std::fixed << std::setprecision(2) << (std::abs(sum) > 1e-6 ? 0 : area);
}