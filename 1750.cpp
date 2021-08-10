/* 1750. Pakhom and the Gully - http://acm.timus.ru/problem.aspx?num=1750
 *
 * Strategy:
 * Try every way to construct a path using all the given points, and report the shortest path that
 * doesn't cross the gully.
 *
 * Performance:
 * O(n), runs the tests in 0.125s using 300KB memory.
 */

#include <iostream>
#include <vector>
#include <iomanip>

using ll = long long;
using ld = long double;

struct point {
    ll x, y;
    point operator- (const point& b) const { return { x - b.x, y - b.y }; }
    ld len() const { return std::sqrt(x*x + y*y); }
    ll operator% (const point& b) const { return x*b.y - b.x*y; } // "Cross product", turn
};

struct segment {
    point s, e;
};

bool in(ll a, ll b, ll c, bool open) { // Returns true if c is in (a,b) if open else [a, b]
    return open ? std::min(a, b) < c && c < std::max(a, b)
                : std::min(a, b) <= c && c <= std::max(a, b);
}

bool intersects(const segment& a, const segment& b, bool open) {
    // Intersects segment a and b, not counting endpoints if open is true
    auto x = (b.s - a.s) % (a.e - a.s), y = (b.e - a.s) % (a.e - a.s);
    auto z = (a.s - b.s) % (b.e - b.s), w = (a.e - b.s) % (b.e - b.s);
    if(x*y == 0 && z*w == 0)
        // One segment has at least one point on the others' extended line so we check if any point
        // is actually within the proper interval of the segment
        return !((b.s-a.s)%(b.s-a.e)) && in(a.s.x,a.e.x,b.s.x,open) && in(a.s.y,a.e.y,b.s.y,open) ||
               !((b.e-a.s)%(b.e-a.e)) && in(a.s.x,a.e.x,b.e.x,open) && in(a.s.y,a.e.y,b.e.y,open);
    return open ? x*y < 0 && z*w < 0 : x*y <= 0 && z*w <= 0; // The segments straddle each other
}

ld shortest(std::vector<point> u, std::vector<std::vector<point>> v)  {
    // Finds the shortest paths among v to cross the gully u
    ld shortest = std::numeric_limits<long double>::infinity();

    for(auto c : v) {
        bool is = false;
        for(int i = 0; i < u.size() - 1; i++) {
            for(int j = 0; j < c.size() - 1; j++) {
                // We consider the line segments closed if we test the middle vertex of the gully
                // and the gully isn't all on one side of the path
                bool open = !(i==1 && ((c[j+1]-c[j])%(u[2]-c[j]))*((c[j+1]-c[j])%(u[0]-c[j])) < 0);
                is |= intersects( segment { u[i], u[i+1] }, segment { c[j], c[j+1] }, open);
            }
        }
        
        ld sum = 0;
        if(!is) { // The path circumnavigated the gully successfully, update the shortest path
            for(int j = 0; j < c.size() - 1; j++)
                sum += (c[j] - c[j+1]).len();
            if(sum < shortest)
                shortest = sum;
        }
    }
    return shortest;
}

int main() {
    point S, T, A, B, C;
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n;
    std::cin >> n;
    while(n--) {
        std::cin >> S.x >> S.y >> T.x >> T.y >> A.x >> A.y >> B.x >> B.y >> C.x >> C.y;
        auto g = { A, B, C };
        std::vector<std::vector<point>> d = { // All the ways to cross the gully, good or bad
            { S, A, B, T },
            { S, B, C, T },
            { S, C, A, T },
            { S, B, A, T },
            { S, C, B, T },
            { S, A, C, T },
            { S, A, T },
            { S, B, T },
            { S, C, T },
            { S, T }
        };
        auto s = shortest(g, d);
        std::cout << std::fixed << std::setprecision(10) << s << std::endl;
    }
}
