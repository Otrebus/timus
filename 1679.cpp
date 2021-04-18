/* 1679. Scrooge's Tower - http://acm.timus.ru/problem.aspx?num=1679
 *
 * Strategy:
 * If the points given were on the same wall, then they would need to form a line that is tangent
 * to the circle with radius R/sqrt(2), which is the circle inscribed within the tower and has the
 * tower wall as its tangents. If the second point is not on the same wall as the first one
 * we can check where the equivalent point would be on the first wall by rotating the second point
 * 90 degrees until it forms a tangent to said circle with the first one, unless that never occurs
 * which means we output "NO".
 *
 * Performance:
 * O(T), runs the tests in 0.015s using 236KB memory.
 */

#include <iostream>

using ll = long long;

bool solve(ll R, ll x1, ll y1, ll x2, ll y2) {
    if(x1 == x2) // This would mess up our calculations so just flip the axes
        std::swap(x1, y1), std::swap(x2, y2);

    for(int i = 0; i < 4; i++) {
        // Rotate the second point
        std::swap(x2, y2);
        y2 = -y2;

        // If the two points are the same, we can just make sure that we are outside the inscribed
        // circle, we can always form a tangent that way
        if(x1 == x2 && y1 == y2)
            return 2*x1*x1 + 2*y2*y2 >= R*R;

        // Check whether the two points are tangent to the inscribed circle - this is derived from
        // substituting y = kx + m into r^2 = x^2 + y^2 and solving for when we get a double root
        ll dy = (y2 - y1), dx = (x2 - x1);
        if(R*R*dy*dy == 2*y1*y1*dx*dx - 4*dx*dy*x1*y1 + 2*dy*dy*x1*x1 - R*R*dx*dx)
            return true;
    }
    return false;
}

int main() {
    ll T, x1, y1, x2, y2, R;

    std::cin >> T;
    while(T--) {
        std::cin >> R >> x1 >> y1 >> x2 >> y2;
        bool b = solve(R, x1, y1, x2, y2);
        std::cout << (b ? "YES" : "NO") << std::endl;
    }
}
