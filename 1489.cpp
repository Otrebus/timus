/* 1489. Points on a Parallelepiped - http://acm.timus.ru/problem.aspx?num=1489
 *
 * Strategy:
 * Straightforward mapping/transformation.
 *
 * Performance:
 * 0.015s, 472KB.
 */

#include <iostream>
#include <functional>
#include <iomanip>
#include <cmath>

struct vec3d
{
    double x, y, z;
};

class Surface;
typedef std::function<vec3d(const Surface&, double, double)> ft;
const double delta = -1e-8;

class Surface
{
public:
    double x1, x2, y1, y2;
    ft f;

    Surface(int x1, int x2, int y1, int y2, ft f) : x1(x1), y1(y1), x2(x2), y2(y2), f(f) 
    { 
    }

    bool inBounds(double x, double y)
    {
        return x - x1 > delta && x2-x > delta && y - y1 > delta && y2 - y > delta;
    }

    vec3d transform(double x, double y)
    {
        return f(*this, x, y);
    }
};

int main()
{
    int A, B, C;      
    std::pair<double, double> pi[2];
    vec3d po[2];
    std::cin >> A >> B >> C >> pi[0].first >> pi[0].second >> pi[1].first >> pi[1].second;

    auto top = Surface(C, C+A, B+C, 2*B+C, [] (const Surface& s, double x, double y) 
                                         { return vec3d { x - s.x1,  s.x1, s.y2 - y }; } );
    auto left = Surface(0, C, B+C, 2*B+C, [] (const Surface& s, double x, double y) 
                                         { return vec3d { 0, x, s.y2 - y }; } );
    auto right = Surface(C+A, 2*C+A, B+C, 2*B+C, [] (const Surface& s, double x, double y) 
                                         { return vec3d { s.x1*2 - s.x2, s.x2 - x, s.y2 - y }; } );
    auto back = Surface(C, C+A, 2*B+C, 2*B+2*C, [] (const Surface& s, double x, double y) 
                                         { return vec3d { x - s.x1, s.y2 - y, 0 }; } );
    auto front = Surface(C, C+A, B, B+C, [] (const Surface& s, double x, double y) 
                                         { return vec3d { x - s.x1, y - s.y1, s.y1 }; } );
    auto bottom = Surface(C, C+A, 0, B, [] (const Surface& s, double x, double y) 
                                         { return vec3d { x - s.x1, 0, y }; } );
    Surface v[6] { top, left, right, back, front, bottom };

    for(int i = 0; i < 2; i++)
        for(auto& s : v)
            if(s.inBounds(pi[i].first, pi[i].second))
                po[i] = s.transform(pi[i].first, pi[i].second);

    std::cout << std::fixed << std::setprecision(10)
              << std::sqrt((po[0].x-po[1].x)*(po[0].x-po[1].x)+
                           (po[0].y-po[1].y)*(po[0].y-po[1].y)+(po[0].z-po[1].z)*(po[0].z-po[1].z));
}