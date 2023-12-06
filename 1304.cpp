/* 1304. Parallelepiped - http://acm.timus.ru/problem.aspx?num=1304
 *
 * Strategy:
 * For each pair of z-coordinates given in the points and boundaries, project all points between
 * those z-coordinates onto the xy plane and find the largest empty rectangle in quadratic time;
 * the answer is the maximum volume formed by the parallelepiped formed by any such z-interval and
 * its maximum rectangle.
 *
 * Performance:
 * O(n^4), runs the tests in 0.015s using 200KB memory.
 */

#include <stdio.h>
#include <algorithm>
#include <vector>
#include <iterator>
#include <unordered_set>

using ld = long double;
using ll = long long;
ll u, v, w, ans = 0;
int n;
struct point { ll x, y, z; };
std::vector<point> P;

ll calc(ll x1, ll x2, ll y1, ll y2, ll z1, ll z2) {
    // Calculates the area of the largest empty rectangle in the xy plane inside the given
    // bounding box with all points between z1 and z2 projected into it
    std::vector<point> v;
    auto fn = [&z1, &z2](auto& p) { return p.z > z1 && p.z < z2; };
    std::copy_if(P.begin(), P.end(), std::back_inserter(v), fn);

    auto n = v.size();
    if(!n)
        return (y2-y1)*(x2-x1);

    // Find rectangles between the top and bottom sides by finding the smallest span between
    // any x coordinates and multiplying by the height
    std::sort(v.begin(), v.end(), [] (const point& a, const point& b) { return a.x < b.x; });
    ll w = std::max(v[0].x-x1, x2-v.back().x);
    for(int i = 1; i < n; i++)
        w = std::max(w, v[i].x-v[i-1].x);
    ll maxb = (y2-y1)*w;

    // Find rectangles between points, and between points and the bottom
    std::sort(v.begin(), v.end(), [] (const point& a, const point& b) { return a.y > b.y; });
    for(int i = 0; i < n; i++) {
        ll l = x1, r = x2;
        for(int j = i+1; j < n; j++) {
            // We start with a rectangle that spans the x-width of the xy plane starting at v[i]
            // and then grows downwards but gets chopped off by each v[j] below; this finds every
            // rectangle that has its top edge at v[i] and left/right edges bounded by other points
            if(v[j].x > l && v[j].x < r) {
                maxb = std::max(maxb, (r-l)*(v[i].y-v[j].y));
                if(v[j].x >= v[i].x)
                    r = std::min(r, v[j].x);
                else if(v[j].x <= v[i].x)
                    l = std::max(l, v[j].x);
            }
        }
        // Finally multiply by the distance to the bottom
        maxb = std::max(maxb, (r-l)*(v[i].y-y1));
    }

    // Find rectangles between the top and spanning as far as possible dowwnards between each pair
    // of other points
    for(int i = v.size()-1; i >= 0; i--) {
        ll l = x1, r = x2;
        for(int j = i-1; j >= 0; j--) {
            if(v[j].x >= v[i].x)
                r = std::min(r, v[j].x);
            else if(v[j].x <= v[i].x)
                l = std::max(l, v[j].x);
        }
        maxb = std::max(maxb, (r-l)*(y2-v[i].y));
    }

    return maxb;
}

ll div(ll ans) { // Rounds the integers into fixed-point output
    return (ans+5000)/10000;
}

int main()
{
    ld x, y, z;
    scanf("%lf %lf %lf %d", &x, &y, &z, &n);

    // We turn all the input into integers by multiplying by 100
    u = x*100 + 0.1, v = y*100 + 0.1, w = z*100 + 0.1;
    std::unordered_set<ll> S; // All the z-coordinates
    for(int i = 0; i < n; i++) {
        scanf("%lf %lf %lf", &x, &y, &z);
        ll a = x*100+0.1, b = y*100+0.1, c = z*100+0.1;
        if(a >= 0 && a <= u && b >= 0 && b <= v && c >= 0 && c <= w) {
            P.push_back({ a, b, c });
            S.insert(c);
        }
    }

    S.insert({ 0, w });
    for(auto z1 : S)
        for(auto z2 : S)
            if(z2 > z1)
                ans = std::max(ans, (z2-z1)*calc(0, u, 0, v, z1, z2));

    ans = div(ans);
    printf("%lld.%02lld\n", ans/100, ans%100);
    return ans/100;
}
