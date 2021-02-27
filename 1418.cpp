/* 1418. Military Story - http://acm.timus.ru/problem.aspx?num=1418
 *
 * Strategy:
 * Count the number of convex layers of the set by doing repeated convex hulls.
 *
 * Performance:
 * O(N^2), runs the tests in 0.062s using 584KB memory.
 */

#include <iostream>
#include <vector>
#include <algorithm>

struct point {
    int x, y;
    bool operator<(const point& b) const {
        return (x == b.x) ? (y > b.y) : (x < b.x);
    }
} p[4000];

bool turnsRight(point a, point b, point c) {
    return (b.x-a.x)*(c.y-a.y) - (c.x-a.x)*(b.y-a.y) < 0;
}

std::vector<point> extract(std::vector<point>& v) { // Extracts the points not in the convex hull
    std::vector<point> ps, ps2, ps3, out; // Partial and full convex hulls and the output

    for(auto it = v.begin(); it < v.end(); ps.push_back(*it), it++) // Lower hull part
        for(int i = ps.size() - 2; i >= 0 && turnsRight(ps[i], ps[i+1], *it); i--)
            ps.pop_back();

    for(auto it = v.rbegin(); it < v.rend(); ps2.push_back(*it), it++) // Upper hull part
        for(int i = ps2.size() - 2; i >= 0 && turnsRight(ps2[i], ps2[i+1], *it); i--)
            ps2.pop_back();

    // Merge the convex hull parts
    std::merge(ps.begin(), ps.end(), ps2.rbegin(), ps2.rend(), std::back_inserter(ps3));
    // Subtract it from the input set
    std::set_difference(v.begin(), v.end(), ps3.begin(), ps3.end(), std::back_inserter(out));
    return out;
}

bool isHullable(std::vector<point>& v) {
    // Returns true if there are any points in the set that don't lie in a straight line
    bool is = false;
    for(int i = 0; i < ((int)v.size()) - 2; i++) {
        is = is || (turnsRight(v[i], v[i+1], v[i+2]) || turnsRight(v[i+2], v[i+1], v[i]));
    }
    return is;
}

int main() {
    int n, ans = 0;
    std::cin >> n;
    std::vector<point> v;

    for(int i = 0; i < n; i++) {
        int x, y;
        std::cin >> x >> y;
        v.push_back({x, y});
    }

    std::sort(v.begin(), v.end());
    while(isHullable(v))
        ans++, v = extract(v);
    std::cout << ans;
}

