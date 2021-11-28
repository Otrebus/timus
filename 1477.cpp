/* 1477. Airplanes - http://acm.timus.ru/problem.aspx?num=1477
 *
 * Strategy:
 * For each pair of points (airplanes), look at the side(s) of the sphere with those two points on
 * the rim. Then slightly tilt the view so that all points on one half of the rim are visible (along
 * with those facing us on the sphere). In order to tilt in every direction, use a sweeping method.
 *
 * Performance:
 * O(N^3*log N), runs the tests in 0.046s using 772KB memory.
 */

#include <stdio.h>
#include <cmath>
#include <vector>
#include <algorithm>
#include <unordered_set>

using ld = long double;
using ll = long long;
const ld pi = std::acos(-1.0l);

ll gcd(ll a, ll b) { return !b ? a : gcd(b, a % b); }
ll gcd(ll a, ll b, ll c) { return gcd(a, gcd(b, c)); }

struct vec { // Standard 3d vector stuff
    ll x, y, z;
    vec operator% (const vec& v) const { return { y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x }; }
    ll operator* (const vec& v) const { return x*v.x + y*v.y + z*v.z; }
    bool operator== (const vec& v) const { return v.x==x && v.y==y && v.z==z; }
    bool s(const vec& v) { return (*this)%v == vec { 0, 0, 0 }; }
    vec operator/ (const ll f) const { return { x/f, y/f, z/f }; }
    ld d() const { return std::sqrt(x*x + y*y + z*z); }
} P[200];
int N;

ld angl(const vec& u, const vec& n, const vec& a) {
    // The angle of a with respect to u, with n as the normal
    ld t = std::acos((a*u)/(a.d()*u.d()));
    ll d = (u%a)*n;
    if(!d)
        return u*a > 0 ? 0 : pi;
    return d > 0 ? t : -t;
}

bool cmp(const vec& u, const vec& n, const vec& a, const vec& b) {
    // Compares the angles of two vectors a and b with respect to u, with n as the normal
    return angl(u, n, a) < angl(u, n, b);
}

struct hash { // Used to hash normal vectors to keep track of what side of the sphere we've checked
    std::size_t operator()(const vec &v) const { return (v.x ^ (v.y*7517) ^ (v.z*27644437)); }
};
std::unordered_set<vec, hash> H;

int calc(int i, int j) {
    // Calculates the maximum number of points visible with points i and j on the rim
    vec& u = P[i], &v = P[j];
    vec n = u%v; // Normal
    int up = 0, arc = 1; // Number of points facing us and on the arc

    if(u.s(v) || H.find(n/gcd(n.x, n.y, n.z)) != H.end()) // Already checked this side of the sphere
        return 0;

    std::vector<vec> V; // The points on the ring
    for(int i = 0; i < N; i++) {
        if(P[i]*n > 0) // All the points facing us
            up++;
        else if(P[i]*n == 0) // All the points on the ring
            V.push_back(P[i]);
    }
    std::sort(V.begin(), V.end(), [&u,&n] (const vec& a, const vec& b) { return cmp(u,n,a,b); });
    
    int sz = V.size(), r = 0;
    while(r+1 < sz && (V[0]%V[(r+1+sz)%sz])*n > 0)
        r++, arc++;

    int maxarc = arc; // The maximum number of points on the rim
    for(int l = 1; l < sz; l++) { // Sweep by incrementing the leftmost point on the rim
        arc--; // We drop that point ..
        while((V[l]%V[(r+1)%sz])*n > 0) // .. and add points on the right side
            arc++, r++;
        maxarc = std::max(arc, maxarc);
    }

    H.insert(n/gcd(n.x, n.y, n.z)); // Note that we already checked this side of the sphere

    return std::max(up + maxarc, N - (up + sz) + maxarc); // We also consider the other side
}

int main() {
    scanf("%d", &N);
    for(int i = 0; i < N; i++)
        scanf("%lld %lld %lld", &P[i].x, &P[i].y, &P[i].z);

    // A couple of special cases that doesn't work with our algorithm
    if(N == 1)
        return printf("1"), 0;
    if(N == 2)
        return printf("%d", (P[0]%P[1] == vec { 0, 0, 0 }) ? 1 : 2), 0;

    int ans = 0;
    for(int i = 0; i < N; i++)
        for(int j = i+1; j < N; j++)
            ans = std::max(ans, calc(i, j));
    printf("%d", ans);
}
