/* 1956. Fire Signals - http://acm.timus.ru/problem.aspx?num=1956
 *
 * Strategy:
 * The distance D from a point p to a line crossing through the origin is D = |n*p| where n is the
 * normal vector of the line. If we sum over a set of distances over a set of points, ΣD is equal to
 * 
 *   (p_1 + p_2 + ...)*n - (q_1 + q_2 + ...)*n,
 * 
 * where the p_k are points whose dot product with n are positive, and the q_k are points whose dot
 * products are negative. We can then employ an sweepline algorithm where we pick an origin point,
 * sort the other points by angle and rotate a line between the origin and each point while
 * updating the sum of the coordinates of the points on the positive side and negative side to
 * quickly calculate each above sum to find the minimum of the total distance.
 * 
 * Performance:
 * O(n^2log n), runs the tests in 0.781s using 204KB memory.
 */

#include <stdio.h>
#include <cmath>
#include <algorithm>

using ld = long double;
using ll = long long;

const ld inf = std::numeric_limits<ld>::infinity();
ld ans = inf;

enum { Neg, Opp, Pos, Prl }; // Point classification relative to the line

struct point {
    ll x, y;
    point operator-(const point& p) const { return { x-p.x, y-p.y }; };
    ll operator%(const point& p) { return x*p.y - p.x*y; }
    ll operator*(const point& p) { return x*p.x + y*p.y; }
    point& operator+=(const point& p) { return *this = { x+=p.x, y+=p.y }; };
    point& operator-=(const point& p) { return *this = { x-=p.x, y-=p.y }; };
    bool operator!=(const point& p) { return p.x != x || p.y != y; };
    ld d() const { return std::sqrt(x*x + y*y); }
    point p() const { return { y, -x }; }
} P[1000], Q[1000];

point sum[2]; // Negative/positive plane point sum
int C[1000]; // Classification of each point

int n, m;

bool classify(const point& c, int i, int j) {
    // Classifies point j relative to the line going through points c and P[i]
    auto u = Q[i]-c, v = Q[j]-c;
    int pre = C[j];

    auto updateSumNeg = [&j, &v] () { if(!C[j]) sum[0] -= v, sum[1] += v; };
    auto updateSumPos = [&j, &v] () { if(C[j]) sum[1] -= v, sum[0] += v; };

    if(u%v == 0 && u*v > 0)
        updateSumNeg(), C[j] = Prl;
    else if(u%v > 0)
        updateSumPos(), C[j] = Neg;
    else if(u%v < 0)
        updateSumNeg(), C[j] = Pos;
    else if(u%v == 0 && u*v < 0)
        updateSumNeg(), C[j] = Opp;
    return C[j] != pre;
}

// Advances point p (the first point index-wise in the positive plane or opposite Q[i])
int advP(const point& c, int i, int p) {
    auto u = (Q[i]-c);
    for(int a = 0; a <= m; a++) {
        int j = (p+a)%m;
        auto v = Q[j]-c;

        // If this point ended up not positive or opposite, reclassify
        if(u%v > 0 || u%v == 0 && u*v >= 0)
            classify(c, i, j);
        else // Otherwise, return it
            return j;
    }
    return p;
}

// Given c as center point, updates the answer
void calc(const point& c) {
    m = 0;
    for(int i = 0; i < n; i++)
        if(P[i] != c)
            Q[m++] = P[i];

    // Sort function to sort the other points by angle
    auto sortFn = [&c] (const point& a, const point& b) {
        auto va = a - c, vb = b - c;
        return std::atan2(va.y, va.x) < std::atan2(vb.y, vb.x);
    };
    std::sort(Q, Q+m, sortFn);

    // Precalculate the positive and negative plane sums of the coordinates
    sum[0] = sum[1] = { 0, 0 };
    for(int i = 0; i < m; i++)
        sum[0] += Q[i]-c, C[i] = Neg;

    // Pre-classify each point
    for(int j = 0; j < m; j++)
        classify(c, 0, j);
    
    // Find the first point p (described above)
    int p = 0;
    for(int a = 0; a < m-1 && (C[a] == Neg || C[a] == Prl); a++)
        p = a+1;

    // Iterate through each point and update the answer
    for(int j = 0; j < m; j++) {
        p = advP(c, j, p);
        classify(c, j, j);
        auto n = (Q[j]-c).p();
        ans = std::min(ans, (sum[1] - sum[0])*n/n.d());
    }
}

int main() {
    scanf("%d", &n);

    for(int i = 0; i < n; i++)
        scanf("%lld %lld", &P[i].x, &P[i].y);

    for(int i = 0; i < n; i++)
        calc(P[i]);

    printf("%lf\n", (ans == inf) ? 0 : ans);
}
