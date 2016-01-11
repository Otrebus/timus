/* 1754. Explosion in a Pyramid - http://acm.timus.ru/problem.aspx?num=1745
 *
 * Strategy:
 * Point/plane distance calculation.
 *
 * Performance:
 * O(1), runs the test cases in 0.015s using 404KB memory.
 */

#include <iostream>

struct vec3d { long long x, y, z; } p[5];

// Returns true if 'radius' is greater than the distance from point c to the plane spanned 
// by points p1, p2 and p3
bool isLarger(long long radius, vec3d c, vec3d p1, vec3d p2, vec3d p3 )
{
    if(radius < 0)
        return false;
    vec3d v1 = vec3d { p1.x-p3.x, p1.y-p3.y, p1.z-p3.z };
    vec3d v2 = vec3d { p2.x-p3.x, p2.y-p3.y, p2.z-p3.z };
    long long A = v1.y*v2.z - v1.z*v2.y;
    long long B = v1.z*v2.x - v1.x*v2.z;
    long long C = v1.x*v2.y - v1.y*v2.x;
    long long D = -A*p3.x - B*p3.y - C*p3.z;
    long long n = A*c.x + B*c.y + C*c.z + D;
    // Converting to double here since I'm not sure that 64-bit integers are quite enough D:
    return (double)(radius*radius)*1.000000001 >= (double)(n)*(double)(n)
                                                  /((double)(A*A + B*B + C*C));
}

int main()
{
    int m;
    std::cin >> m >> p[0].z >> p[4].z;
    for(int i = 0; i < 5; i++)
        std::cin >> p[i].x >> p[i].y;
    bool b = m >= 2*p[0].z; // If the explosion reaches the floor
    for(int i = 0; i < 3; i++)
        b = b || isLarger(m-p[0].z, p[0], p[1+i%3], p[1+(i+1)%3], p[4]);
    std::cout << (b ? "YES" : "NO");
}