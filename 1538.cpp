/* 1538. Towers of Guard - http://acm.timus.ru/problem.aspx?num=1538
 *
 * Strategy:
 * Randomized brute-force. Randomly pick five points and see if they form a simple convex pentagon.
 * If this fails enough times, we can be very certain there are no such points.
 *
 * Performance:
 * O(1), runs the tests in 0.015s using 504KB memory.
 */

#include <random>
#include <iostream>
#include <vector>

using ld = long double;
using ll = long long;
ld pi = 3.141592653589793238462643383279502884197169399375105820974;

struct pos { ll x, y; };

pos A[5000];

bool test(std::vector<ll>& v) {
    ld sum = 0; // Sum of the interior angles
    for(int i = 1; i <= v.size(); i++) {
        ll a = v[i-1], b = v[i%5], c = v[(i+1)%5];
        ll x1 = A[a].x - A[b].x, x2 = A[c].x - A[b].x;
        ll y1 = A[a].y - A[b].y, y2 = A[c].y - A[b].y;

        if(x1*y2-x2*y1 > 0) // Turns in the wrong direction
            return false;
        sum += std::acos((ld) ((x1*x2 + y1*y2)/(std::sqrt(x1*x1+y1*y1)*std::sqrt(x2*x2+y2*y2))));
    }
    return std::abs(sum - 3*pi) < 1e-8; // Interior angle sum needs to be 540 degrees
}

int main() {
    int N;
    std::cin >> N;

    std::default_random_engine generator(0);
    std::uniform_int_distribution<int> ngen(0, N-1);

    for(int i = 0; i < N; i++) {
        std::cin >> A[i].x >> A[i].y;
    }
    for(int i = 0; i < 5000; i++) {
        std::vector<ll> v;
        while(v.size() != 5) {
            int x = ngen(generator);
            if(std::find(v.begin(), v.end(), x) == v.end())
                v.push_back(x);
        }
        if(test(v)) {
            std::cout << "Yes" << std::endl;
            for(auto x : v)
                std::cout << (x+1) << " ";
            return 0;
        }
    }
    std::cout << "No";
}
