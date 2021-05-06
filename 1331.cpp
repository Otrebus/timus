/* 1331. Vladislava - http://acm.timus.ru/problem.aspx?num=1331
 *
 * Strategy:
 * Turn the given long/lat into cartesian unit vectors and brute-force find the minimum dot product.
 *
 * Performance:
 * O(NM), runs the tests in 0.234s using 540KB memory.
 */

#include <iostream>
#include <vector>
#include <algorithm>

using ld = long double;
const ld pi = 3.141592653589793238462643383279502884L;

struct vec {
    ld x, y, z;
    ld operator* (const vec& v) const { return x*v.x + y*v.y + z*v.z; };
};

vec l2vec(ld lat, ld lng) { // Turns the given lat/long into a unit vector
    lng = lng*pi/180, lat = lat*pi/180;
    return vec { std::cos(lng)*std::cos(lat), std::sin(lng)*std::cos(lat), std::sin(lat) };
}

int main() {
    int N, M;
    ld R, w, l;
    std::vector<vec> g;

    std::cin >> N >> M >> R;

    for(int i = 0; i < M; i++) {
        std::cin >> w >> l;
        g.push_back(l2vec(w, l));
    }

    for(int i = 0; i < N; i++) {
        std::cin >> w >> l;
        vec a = l2vec(w, l);
        vec x = *std::min_element(g.begin(), g.end(), [a] (vec& x, vec& y) { return (x*a > y*a); });
        // We apparently need to clamp the dot product due to precision errors
        std::cout << R*std::acos(std::min(1.0l, std::max(a*x, -1.0l))) << std::endl;
    }
}
