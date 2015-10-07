/* 1111. Squares - http://acm.timus.ru/problem.aspx?num=1111
 *
 * Strategy:
 * Straightforward computations of point/line distances using the standard vector projection method.
 *
 * Performance:
 * Linearithmic in the input, running the tests in 0.001s using 400KB memory.
 */

#include <algorithm>
#include <vector>
#include <iostream>

class Point // Or Vector2d, if you want
{
public:
    double x, y;
    Point(double x, double y) : x(x), y(y) { }
    Point() { }

    Point  operator- (const Point & p) const { return Point(x-p.x, y-p.y);  }
    bool   operator==(const Point & p) const { return p.x == x && p.y == y; }
    Point  operator+ (const Point & p) const { return Point(x+p.x, y+p.y);  }
    Point  operator/ (long long t)     const { return Point(x/t, y/t);      }
    Point  operator* (const double& t) const { return Point(x*t, y*t);      }
    double operator* (const Point& p)  const { return x*p.x + y*p.y;        }

    double length2()                   const { return x*x + y*y;            }

    double dist2(Point v, Point w) // Distance-squared to the line between v and w
    {
        Point p = { x, y };
        Point wv = w-v;
        Point pv = p-v;
        float res = (wv*((pv*wv)/(wv.length2()))-pv).length2();
        return res;
    }
};

class Square
{
public:
    std::vector<Point> points;

    Square(long long x1, long long y1, long long x2, long long y2)
    {
        Point a(x1, y1), b, c(x2, y2), d;
        // The opposite points are given, so the remaining two vectors are calculated by taking the
        // half-way vector between those two points and adding it to itself, rotated +-90 degrees
        Point cent = (c-a)/2;
        b = a + cent + Point { -cent.y, cent.x };
        d = a + cent - Point { -cent.y, cent.x };
        points = { a, b, c, d };
    }

    double dist2(Point p) const
    {
        double min = double(std::numeric_limits<double>::infinity());
        bool inside = true;
        for(long long i = 0; i < 4; i++) // For each side of the square
        {
            Point w = points[(i+1)%4];
            Point v = points[i];

            if(v == w) // Special case of a point square
            {
                min = std::min(min, (v-p).length2());
                continue;
            }

            Point u = w-v;
            Point P = p-v;
            // We're inside the square if all sides of the square are to the right
            // of the query point, in a cross-product sense
            inside &= double(0) < (P.x*u.y-P.y*u.x); 

            double t = (p-v)*(w-v)/(w-v).length2();
            if(t < double(0.0) || double(1.0) < t)
                min = std::min(min, std::min((w-p).length2(), (v-p).length2()));
            else
                min = std::min(p.dist2(v, w), min);
        }
        return inside ? 0 : min;
    }
};

int main()
{
    int n;
    std::cin >> n;
    std::vector<std::pair<int, Square>> v; // { id, square }
    for(int i = 1; i <= n; i++)
    {
        int x1, y1, x2, y2;
        std::cin >> x1 >> y1 >> x2 >> y2;
        v.push_back( { i, Square(x1, y1, x2, y2) } );
    }
    int px, py;
    std::cin >> px >> py;
    Point p(px, py);
    
    // Sort our input according to the distance
    std::sort(v.begin(), v.end(), [&p] (const std::pair<int, Square>& a, 
                                        const std::pair<int, Square>& b) 
                                 { 
                                     double dista = a.second.dist2(p), distb = b.second.dist2(p);
                                     return dista == distb 
                                                ? a.first < b.first 
                                                : a.second.dist2(p) < b.second.dist2(p); 
                                 } );

    for(int i = 0; i < v.size(); i++)
        std::cout << v[i].first << " ";
}