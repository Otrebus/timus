/* 1539. Intelligence Data - http://acm.timus.ru/problem.aspx?num=1539
 *
 * Strategy:
 * Brute-force. For every number of goblins n, check if for each rounded number r we have some m
 * where r-5*10^-(d+1) <= m/n < r+5*10^-(d+1) or equivalently, if we multiply each side by n, we
 * need to have the resulting leftmost expression floor to another number than the rightmost or
 * be a whole number.
 * 
 * Performance:
 * O(10^d), runs the tests in 0.015s using 152KB memory.
 */

#include <stdio.h>

using ll = long long;

ll p[10] = { 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000 };
ll n[100];
ll d, N;

bool testb(ll x, ll a) {
    return a*(x*10-5)%d <= 0 || a*(x*10-5)/d < a*(x*10+4)/d || // <= 0 because of the x=0 edge case
           a*(x*10+4)/d < a*(x*10+5)/d && a*(x*10+5)%d; // Special case since .5 rounds up
}

bool testa(ll a) {
    for(ll i = 0; i < N; i++)
        if(!testb(n[i], a))
            return false;
    return true;
}

int main() {
    scanf("%lld %lld", &d, &N);
    d = p[d+1];
    for(ll i = 0; i < N; i++)
        scanf("%*lld%*c%lld", &n[i]);
    for(ll a = 1; ;a++)
        if(testa(a))
            return printf("%lld", a), 0;
}
