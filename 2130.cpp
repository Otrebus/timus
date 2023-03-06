/* 2130. Alice + Bob - http://acm.timus.ru/problem.aspx?num=2130
 *
 * Strategy:
 * Keep track of the least common multiple of all the allowed divisors, along with all the divisors
 * answered as disallowed. For any new allowed divisor, check that a divisor of it wasn't mentioned
 * as disallowed, and also that the lcm isn't divisible by new disallowed divisors. Always answer
 * the lcm if we're still consistent.
 *
 * Performance:
 * O(N*sqrt(max{d})), runs the tests in 0.171s using 1,152KB memory.
 */

#include <algorithm>
#include <numeric>

using ll = unsigned long long;
const int maxn = 1e3;
const ll limit = 1e12;

bool B[maxn*maxn+1];
bool good = true;
ll N, d, t, l = 1;


ll lcm(ll a, ll b) {
    return a*b/(std::gcd(a, b));
}

void add(ll d) {
    if((l = lcm(d, l)) > limit || B[d])
        good = false;
    for(int i = 1; i*i <= maxn*maxn; i++)
        if(B[i] && d % i == 0)
            good = false;
}

bool check(ll d) {
    B[d] = true;
    return l % d;
}

long long guess() {
    return good ? l : -1;
}

int main() {
    scanf("%llu", &N);
    while(N--) {
        scanf("%llu %llu", &d, &t);
        if(t == 1 && good)
            add(d);
        else
            good &= check(d);
        printf("%lld\n", guess());
    }
}
