/* 1686. Photovoltaic Spaceship - http://acm.timus.ru/problem.aspx?num=1686
 *
 * Strategy:
 * Elementary geometry (vector and dot products).
 *
 * Performance:
 * O(n), runs the tests in 0.001s using 292KB memory.
 */

#include <iostream>
#include <iomanip>

using ld = long double;

struct vec {
    ld x, y, z;
    vec operator% (const vec& v) { return { y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x }; }
    vec operator- (const vec& v) { return { x - v.x, y - v.y, z - v.z }; }
    vec operator+ (const vec& v) { return { x + v.x, y + v.y, z + v.z }; }
    vec operator/ (const ld t) { return { x/t, y/t, z/t }; }
    ld operator* (const vec& v) { return x*v.x + y*v.y + z*v.z; }
    vec operator- () { return { -x, -y, -z }; }
} v[4];

vec N[4], M, s; // Normals, centroid, scrap vector
int n, i;
ld ans;

void read(int i, int a, int b, int c) {
    // Adds a normal from the given vertices, makes sure it points outwards
    N[i] = (v[b]-v[a])%(v[c]-v[a]);
    if(N[i]*(M-v[b]) > 0)
        N[i] = -N[i];
}

int main() {
    for(int i = 0; i < 4; i++)
        std::cin >> v[i].x >> v[i].y >> v[i].z;

    M = (v[0] + v[1] + v[2] + v[3])/4;
    for(int i = 0; i < 3; i++) // Add a normal for each of the four faces
        read(i, 0, i+1, 1+(i+1)%3);
    read(3, 3, 1, 2);

    // Add the dot product of each normal with the vector to the star to the answer, if positive
    for(std::cin >> n; i < n; i++) {
        std::cin >> s.x >> s.y >> s.z;
        for(int i = 0; i < 4; i++)
            ans += std::max(s*N[i], 0.l);
    }
    std::cout << std::fixed << std::setprecision(10) << ans;
}
