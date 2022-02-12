/* 2105. Alice and Bob are on Bikes - http://acm.timus.ru/problem.aspx?num=2105
 *
 * Strategy:
 * All that matters is the relative distance travelled by Alice and Bob, so we multiply the total
 * time they spent moving with their velocities, and divide it by the length of one lap to get the
 * number of laps they performed relative to each other.
 *
 * Performance:
 * O(n), runs the tests in 0.015s using 156KB memory.
 */

#include <stdio.h>

long long L, T, va, vb, n, t[2], i, ti, w, d;

int main() {
    scanf("%lld %lld %lld %lld %lld", &L, &T, &va, &vb, &n);
    while(n--)
        scanf("%lld %*lld %lld", &i, &d), t[i-1] += d;
    printf("%lld", (va*(T-t[0]) + vb*(T-t[1]))/L);
}
