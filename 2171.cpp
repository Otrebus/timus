/* 2171. Two Progressions 2 - http://acm.timus.ru/problem.aspx?num=2171
 *
 * Strategy:
 * For any given pair of sequences we can calculate the coefficients of its union sequence (which
 * expresses their coinciding elements) by solving the Diophantine equation b1 + d1*x = b2 + d2*x.
 * For any sequence we can then calculate the cardinality of any union of sequences by recursively
 * applying the exclusion/inclusion formula.
 *
 * Performance:
 * O(2^K), runs the tests in 0.078s using 6,324KB memory.
 */

#include <stdio.h>
#include <cmath>

using ll = long long;

const ll maxk = 18;
ll B[maxk], D[maxk]; // The coefficients of the input
ll N, K;
const ll maxm = 1 << maxk; // We express subsets of the sequences as bitmasks
std::pair<ll, ll> P[maxm]; // Sequence intersections expressed as sequence coefficients
ll C[maxm], R[maxm]; // Bitcount, R maps a single bit to an index of its sequence

ll gcd(ll a, ll b, ll& x, ll& y) { // Extended euclidean algorithm
    if(b == 0)
        return x = 1, y = 0, a;
    else {
        ll d = gcd(b, a % b, x, y);
        ll xr = y;
        y = x - (a/b)*y;
        x = xr;
        return d;
    }
}

// Finds the sequence that expresses a union of sequences
std::pair<ll, ll> find(ll b1, ll d1, ll b2, ll d2) {
    if(b2 > b1) // Find a starting point past b1
        b1 += (b2-b1+d1-1)/d1*d1;
    ll d = b2-b1, a = d1, b = -d2;
    ll x, y;
    ll g = gcd(a, b, x, y);

    if (d % g) // The gcd of (d1, d2) needs to divide the difference between b1, b2
        return { -1, -1 };

    x *= d/g, y *= d/g;
    g = std::abs(g);

    // Solve the Diophantine equation, ake sure that we end up with a positive multiplicative
    // coefficient and that we don't integer overflow
    ll k = std::min((x < 0 ? -(x+b/g+1) : -x)/(b/g), (y < 0 ? y-a/g+1 : y)/(a/g));
    return { std::min(1+N, b1 + a*(x + (b/g)*k)), std::min(1+N, std::abs((a*b)/g)) };
}

std::pair<ll, ll> getP(ll m) { // Given a set of sequences finds its union sequence
    auto [x, g] = P[m];
    if(g) // Already memoized
        return P[m];

    ll h = m&-m, l = m^(m&-m); // Extract the highest bit, and the rest
    // Recursively unionize a single sequence with the rest of the sequences
    auto [b, d] = getP(l);
    auto p2 = b == -1 ? std::make_pair(-1ll, -1ll) : find(b, d, B[R[h]], D[R[h]]);
    return (P[m] = p2);
}

ll count(std::pair<ll, ll> p) { // Counts the numbers in a sequence in [1, N]
    auto [b, d] = p;
    return b == -1 ? 0 : std::max((1+N-b+d-1)/d, 0ll);
}

int main() {
    scanf("%lld %lld", &N, &K);

    P[0] = { -1, -1 };
    for(ll m = 0; m < (1 << K); m++) // Calculate bitcounts
        for(ll k = 0; k < K; k++)
            if(m & (1 << k))
                C[m]++;
    for(ll k = 0; k < K; k++) // Calculate exact reverse logarithms (to find a sequence from a bit)
        R[1<<k] = k;

    for(ll i = 0; i < K; i++) {
        scanf("%lld %lld", &B[i], &D[i]);
        P[(1<<i)] = { B[i], D[i] }; // Initialize P for single sequences
    }

    ll ans = 0;
    for(ll m = 0; m < (1 << K); m++)
        ans += (-1+2*(C[m]%2))*count(getP(m)); // The inclusion/exclusion formula
    printf("%lld", ans);
}
