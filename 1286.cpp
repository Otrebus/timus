/* 1286. Starship Travel - http://acm.timus.ru/problem.aspx?num=1286
 *
 * Strategy:
 * Using a move of (p, q) followed by a move of (p, -q) we can travel a length of (2p, 0) along the
 * x-axis, and by the same logic a length of 2q along the same axis. This means we can, along one
 * axis, travel (at least) in units of gcd(2p, 2q) = 2*gcd(p, q) = 2g. Since the previous argument
 * is true for the y axis as well, we can (at least) reach points in a grid of cell size 2g. If
 * exactly one of p and q is not divisible by 2g (meaning p or q divided by g is odd) we can also
 * escape to "in between" the 2g grid and reach any point in a grid of cell size g. If both are not
 * divisible by 2g, then we can escape in between the 2g grid exactly in the middle of a grid square
 * and not in between two orthogonal vertices, resulting in a checkerboard pattern. (And, both can't
 * be divisible by 2g since that would contradict the given gcd).
 *
 * Performance:
 * O(log(min(p, q))), runs the tests in 0.015s using 172KB memory.
 */

#include <stdio.h>

using ll = long long;

ll gcd(ll a, ll b) { return !b ? a : gcd(b, a % b); }

int main() {
    ll p, q, x1, y1, x2, y2;
    scanf("%lld %lld %lld %lld %lld %lld", &p, &q, &x1, &y1, &x2, &y2);

    ll dx = x2-x1, dy = y2-y1, g = gcd(p, q);

    // A couple special cases first and we need to make sure dx and dy are at least in the fine grid
    if(!dx && !dy)
        return printf("YES"), 0;
    if(!p && !q || dx % g || dy % g)
        return printf("NO"), 0;

    dx /= g, dy /= g, p /= g, q /= g;
    // If exactly if p, q is odd, or the endpoint is inside the checkerboard
    printf((p&1 ^ q&1 || !((dx+dy)&1)) ? "YES" : "NO");
}
