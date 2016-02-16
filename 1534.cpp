/* 1534. Football in Gondor - http://acm.timus.ru/problem.aspx?num=1534
 *
 * Strategy:
 * Straightforward.
 *
 * Performance:
 * Constant, 0.015s, 240KB.
 */

#include <algorithm>
#include <stdio.h>

long long calcmax(long long atk, long long recv, long long m)
{   // Spread out all wins (across the matches), concentrate the losses
    if(atk >= m)
        return 3*m - (atk-m+1-recv > 0 ? 0 : atk-m+1 == recv ? 2 : 3);
    return atk*3 + m-atk - (recv > 0 ? 1 : 0);
}

long long calcmin2(long long atk, long long recv, long long m)
{   // Spread out all losses, concentrate the wins ..
    if(recv >= m)
        return (atk > recv-m+1 ? 3 : atk == recv-m+1 ? 1 : 0);
    return (atk ? 3 : 1) + (m-1-recv);
}

long long calcmin(long long atk, long long recv, long long m)
{   // .. but try to cancel out one win with one loss first
    long long x = calcmin2(atk, recv, m), y;
    if(m > 1 && recv >= 1 && (y = 1 + calcmin2(atk-1, recv-1, m-1)) < x)
        return y;
    return x;
}

int main()
{
    long long K, L, N;
    scanf("%lld %lld %lld", &K, &L, &N);
    printf("%lld %lld\n", calcmax(K, L, N), calcmin(K, L, N));
}