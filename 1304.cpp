/* 1304. Parallelepiped - http://acm.timus.ru/problem.aspx?num=1304
 *
 * Strategy:
 * Sort the points by their z coordinates and add each x and y coordinate to a set. For each pair
 * of (x, y)-coordinates, iterate (in z-order) over the points inside the axis-aligned box implied
 * by the pair and update the maximum overall volume with the volume of the axis-aligned
 * parallelepiped formed by the box and the z-coordinates of the two most recent points.
 * 
 * Performance:
 * O(n^5), runs the tests in 0.453s using 188KB memory.
 */

#include <stdio.h>
#include <algorithm>
#include <vector>

using ld = long double;
using ll = long long;
ll u, v, w;
int n;
ll ans = 0;

struct point { 
    ll x, y, z; 
    point(ll x, ll y, ll z) : x(x), y(y), z(z) {}
};

std::vector<point> P;
std::vector<ll> X, Y;

void calcZ(ll x1, ll x2, ll y1, ll y2) {
    // Iterates in z-order inside the given box and finds the maximum slice between any consecutive
    // z-coordinates of the points
    ll z1 = 0, z2 = 0;
    for(auto& p : P) {
        if(p.z == 0 || p.z == w || p.y > y1 && p.y < y2 && p.x > x1 && p.x < x2) {
            z1 = z2;
            z2 = p.z;
            ans = std::max(ans, (z2-z1)*(y2-y1)*(x2-x1));
        }
    }
}

ll round(ll ans) {
    // Rounds the fixed-point number in the natural way
    return (ans+5000)/10000;
}

int main()
{
    ld x, y, z;

    // Read stuff and turn them to fixed-point integers
    scanf("%lf %lf %lf %d", &x, &y, &z, &n);
    u = x*100 + 0.1, v = y*100 + 0.1, w = z*100 + 0.1;
    for(int i = 0; i < n; i++) {
        scanf("%lf %lf %lf", &x, &y, &z);
        x = x*100+0.1, y = y*100+0.1, z = z*100+0.1;
        if(x >= 0 && x <= u && y >= 0 && y <= v && z >= 0 && z <= w)
            P.emplace_back(x, y, z);
    }

    P.insert(P.end(), { { 0, 0, 0 }, { u, v, w } }); // Add boundary points

    // Make sorted lists of coordinates and sort points by Z
    for(auto& p : P)
        X.push_back(p.x), Y.push_back(p.y);
    std::sort(X.begin(), X.end());
    std::sort(Y.begin(), Y.end());
    std::sort(P.begin(), P.end(), [] (point& a, point& b) { return a.z < b.z; });
    X.erase(std::unique(X.begin(), X.end()), X.end());
    Y.erase(std::unique(Y.begin(), Y.end()), Y.end());

    // For each pair of (x, y), find the biggest z-slice
    for(int i = 0; i < X.size(); i++)
        for(int j = i+1; j < X.size(); j++)
            for(int k = 0; k < Y.size(); k++)
                for(int l = k+1; l < Y.size(); l++)
                    calcZ(X[i], X[j], Y[k], Y[l]);

    ans = round(ans);
    printf("%lld.%02lld\n", ans/100, ans%100);
}
