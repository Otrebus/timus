/* 1456. Jedi Riddle 2 - http://acm.timus.ru/problem.aspx?num=1456
 *
 * Strategy:
 * Calculate random powers of A until we find a repeated one (a power equal to a previously found
 * one); the answer is a divisor of the difference of their exponents, if it exists.
 *
 * Performance:
 * Runs the tests in 0.078s using 3,724KB memory.
 */

#include <unordered_map>
#include <random>
#include <map>

using ll = long long;

ll A, N;

std::unordered_map<ll, ll> M; // The powers of A and their exponents

ll pow(ll n) {
    // Exponentiation by squaring
    if(!n)
        return 1;
    if(n%2)
        return (A*pow(n-1))%N;
    ll r = pow(n/2);
    return (r*r)%N;
}

const ll inf = 2e9;

ll find(ll d) {
    // Given A^d = 1 (mod N), finds the smallest divisor m of d that gives A^m = 1 (mod N), if any
    ll m = inf;
    for(ll i = 1; i*i <= d; i++) {
        if(pow(i+1) == A && pow(i) == 1)
            m = std::min(m, i);
        if(pow(d/i+1) == A && pow(d/i) == 1)
            m = std::min(d/i, m);
    }
    return m;
}

ll solve() {
    std::random_device rd;
    int seed = rd();
    std::default_random_engine generator(seed);
    std::uniform_int_distribution<ll> ngen(1, N);

    while(true) {
        int p = ngen(generator);
        ll r = pow(p);
        if(r == 1) { // We found A^p = 1 (mod N)
            ll x = find(p);
            return x == inf ? 0 : x;
        }
        else if(M.find(r) != M.end() && M[r] != p) {
            // We found A^p = r (mod N) and A^q = r (mod N)
            ll x = find(std::abs(M[r] - p));
            return x == inf ? 0 : x;
        }
        else
            M[r] = p;
    }
}

int main() {
    scanf("%lld %lld", &A, &N);
    printf("%lld", solve());
}
