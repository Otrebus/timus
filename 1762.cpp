/* 1762. Search for a Hiding-Place - http://acm.timus.ru/problem.aspx?num=1762
 *
 * Strategy:
 * Let m be the longer side (call it horizontal) and determine how many diagonals we form as we
 * move between the vertical sides. Each such diagonal will have one tile flipped by every future
 * horizontal crossing of the room. Also, every bounce against the wall (the "remainder") will be
 * crossed twice per horizontal crossing, except for the final exit, where it will be crossed once.
 *
 * Performance:
 * O(min(n-1, m-1)/gcd(n-1, m-1)), runs the tests in 0.031s using 156KB memory.
 */

#include <stdio.h>
#include <algorithm>
#include <stdlib.h>

using ll = long long;

int main() {
    ll n, m;

    scanf("%lld %lld", &n, &m);
    if(m < n) // Make m the longer side
        std::swap(m, n);

    for(ll i = 0, k = 0, w = 0, ans = 0; ;i++) {
        auto a = div((m-1-k), n-1);
        if(!a.rem)
            return printf("%lld", ans+m-2*w-(i/2)*2), 1; // Final crossing
        ans += m-1 - 2*((i/2)*2 + w); // Flip m-1, unflip w + the number of sidewall bounces
        k = (n-a.rem-1); // The number of tiles into the room we start due to the sidewall bounce
        w += a.quot; // Add the number of diagonals we form as we cross
    }
}
