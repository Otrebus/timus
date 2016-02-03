/* 1600. Airport - http://acm.timus.ru/problem.aspx?num=1600
 *
 * Strategy:
 * This is essentially sphere/ray intersection. Treat one plane as a static sphere of radius d,
 * and the other plane as a ray with direction that of their speed difference, and solve.
 *
 * Performance:
 * O(N^2), runs the test cases in 0.358s using 2480KB memory.
 */

#include <cmath>
#include <iostream>
#include <vector>
#include <algorithm>

class Vector3d
{
public:
    double x, y, z;
    Vector3d(double x, double y, double z) : x(x), y(y), z(z) { }
    Vector3d  operator- (const Vector3d & p) const { return Vector3d(x-p.x, y-p.y, z-p.z); }
    Vector3d  operator/ (double t)           const { return Vector3d(x/t, y/t, z/t); }
    double operator* (const Vector3d& p)     const { return x*p.x + y*p.y + z*p.z; }

    double length()                          const { return std::sqrt(x*x + y*y + z*z); }  
    Vector3d normalized()                    const { double l = length(); return *this/length(); }
};

double collide(std::pair<Vector3d, Vector3d> s1, std::pair<Vector3d, Vector3d> s2, double radius)
{
    Vector3d dir = (s1.second - s2.second).normalized(); // Relative speeds

    // This is standard sphere-ray intersection stuff
    Vector3d vec = s1.first - s2.first;
	double C = vec*vec - radius*radius;
	double B = 2*(vec*dir);
	double A = dir*dir;
	double D = (B*B/(4*A) - C)/A;

	if(D >= -1e-7)
	{
        D = std::max(0.0, D);
        double t = -B/(2*A) - sqrt(D);
        // Negative t means one plane started inside another plane's zone which would be bad input
        return (t > 0) ? t : std::numeric_limits<double>::infinity();
	}
    else
        return std::numeric_limits<double>::infinity();
}

int main()
{
    int n;
    double d;
    std::vector<std::pair<Vector3d, Vector3d>> v;
    std::vector<std::pair<std::pair<int, int>, double>> cols;
    scanf("%d %lf", &n, &d);

    while(n--)
    {
        double a, b, c, d, e, f;
        scanf("%lf %lf %lf %lf %lf %lf", &a, &b, &c, &d, &e, &f);
        v.push_back({{ a, b, c }, { d, e, f }});
    }

    for(int i = 0; i < v.size(); i++)
        for(int j = 0; j < i; j++)
            cols.push_back({{j+1, i+1}, collide(v[i], v[j], d)/(v[i].second-v[j].second).length()});

    auto it = std::min_element(cols.begin(), cols.end(), 
                               [] (std::pair<std::pair<int, int>, double> a,
                                   std::pair<std::pair<int, int>, double> b) 
                                   { return a.second < b.second; } );
    if(it != cols.end() && it->second < std::numeric_limits<double>::infinity())
        printf("ALARM!\n%.3lf %d %d\n", it->second, it->first.first, it->first.second);
    else
        printf("OK\n");
}