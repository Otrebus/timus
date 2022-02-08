/* 2141. Sasha Vilkin - http://acm.timus.ru/problem.aspx?num=2141
 *
 * Strategy:
 * Read the elements online while adding to the sum of the full sequence, and also updating the
 * smallest prefix sums of the two different kinds of sequences (+-+-... vs -+-+...). Form candidate
 * answer sequences ending at the most recently read element and starting at the end of one of the
 * two prefixes.
 *
 * Performance:
 * O(n), runs the tests in 0.031s using 124KB memory.
 */

#include <stdio.h>
#include <algorithm>

int main() {
    long long min[2] = { 0, 0xffffffff }, ans = 0, sum = 0, x, n;
    
    scanf("%lld", &n);
    for(int i = 1; i <= n; i++) {
        scanf("%lld", &x);
        sum += i&1 ? x : -x;
        // Update the prefix whose type matches the sequence starting after this point
        min[i&1] = std::min({ min[i&1], i&1 ? -sum : sum });
        // Update the max by cutting away the two different types of prefixes
        ans = std::max( { ans, -sum-min[1], sum-min[0] } );
    }
    printf("%lld", ans);
}
