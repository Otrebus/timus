/* 1103. Pencils and Circles - http://acm.timus.ru/problem.aspx?num=1103
 *
 * Strategy:
 * Pick two points on the convex hull of the point set. If we form a circle with some other point,
 * the largest such circle will encompass all points, the second largest will encompass all points
 * except for one, etc. The angle formed by the two static points and the third that we pick will
 * be inversely proportional to the radius of the circle that they form (the specific location of
 * the third point on the circle does not affect the angle), so we find the median biggest angle
 * out of all angles that we can form accordingly, and use the points constituting that circle as
 * the answer.
 *
 * Performance:
 * O(n), runs the test suite in 0.031s using 548KB memory.
 */

#include <algorithm>
#include <vector>
#include <iostream>

struct pos { long double x, y; int xi, yi; };

int main()
{
    std::ios::sync_with_stdio(false);
    int n;
    std::cin >> n;
    std::vector<pos> v(n);
    for(int i = 0; i < n; i++)
        std::cin >> v[i].xi >> v[i].yi, v[i].x = v[i].xi, v[i].y = v[i].yi;
    std::swap(*v.begin(), *std::min_element(v.begin(), v.end(), [] (pos v1, pos v2) 
                     { return v1.x == v2.x ? v1.y < v2.y : v1.x < v2.x;}));
    for(int i = 2; i < n; i++)
        if((v[i].x-v[0].x)*(v[1].y-v[0].y) >= (v[i].y-v[0].y)*(v[1].x-v[0].x))
            std::swap(v[i], v[1]);
    auto& a = v[0], &b = v[1];
    std::nth_element(v.begin()+2, v.begin() + 2 + (n-2)/2, v.end(), [a, b] (pos v1, pos v2)
    { 
        pos v1a = { a.x-v1.x, a.y-v1.y }, v1b = { b.x-v1.x, b.y-v1.y };
        pos v2a = { a.x-v2.x, a.y-v2.y }, v2b = { b.x-v2.x, b.y-v2.y };
        return std::acos((v2a.x*v2b.x+v2a.y*v2b.y)
                        /std::sqrt((v2a.x*v2a.x+v2a.y*v2a.y)*(v2b.x*v2b.x+v2b.y*v2b.y)))
             < std::acos((v1a.x*v1b.x+v1a.y*v1b.y)
                        /std::sqrt((v1a.x*v1a.x+v1a.y*v1a.y)*(v1b.x*v1b.x+v1b.y*v1b.y)));
    } );
    auto it = v.begin() + 2 + (n-2)/2;
    std::cout << a.xi <<" "<< a.yi << "\n" << b.xi <<" "<< b.yi << "\n" << it->xi << " " << it->yi;
}