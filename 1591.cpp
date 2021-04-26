/* 1591. Abstract Thinking - http://acm.timus.ru/problem.aspx?num=1591
 *
 * Strategy:
 * We are looking to find the number of different ways that we can configure three chords on a
 * convex polygon to intersect inside the polygon. It helps to break them down by the number of
 * intersections. For three intersections, we picture a hexagon where we form chords between points
 * 1 and 4, 2 and 5, and 3 and 6, numbered clockwise. These points can be chosen in C(n, 6)
 * different ways. For two intersections, we have five points on a pentagon where we connect points
 * 1 and 3, 2 and 4, and 3 and 5, but we are no longer rotationally symmetric so we can rotate this
 * shape five times to bring the number of configurations to 5*C(n, 5). The number for one
 * intersection can be derived similarly to be 4*C(n, 4).
 *
 * Performance:
 * O(1), runs the tests in 0.001s using 272KB memory.
 */

#include <iostream>

using ll = long long;

ll C(ll n, ll k) {
    ll c = 1;
    for(int a = n, b = 1; a > n-k; a--)
        c = (c*a)/b++;
    return n >= k ? c : 0;
}

int main() {
    int n;
    std::cin >> n;
    std::cout << (C(n, 6) + 5*C(n, 5) + 4*C(n, 4));
}
