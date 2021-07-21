/* 1855. Trade Guilds of Erathia - http://acm.timus.ru/problem.aspx?num=1855
 *
 * Strategy:
 * For two cities numbered L and R (with segments between them numbered L to R-1), the total number
 * of trade routes crossing some segment p, multiplied with the cost C_p of that segment is
 * 
 *  (1 + p - L)(R - p)C_p = (R - LR)C_p + p(R + L - 1)C_p - p^2C_p. 
 * 
 * We use a segment tree with lazy propagation to store and query the sum of these C_p, p*C_p and
 * p^2*C_p using their summation formulas, in order to quickly calculate the sum of the above
 * formula for any given range (a, b).
 *
 * Performance:
 * O(mnlog n), runs the tests in 0.265s using 6,464KB memory.
 */

#include <stdio.h>
#include <array>

const int maxn = 262144;
using ld = long double;
using ll = long long;

struct ud { // The range sums
    ll a0, a1, a2;
    ud operator+ (const ud& b) const {
        return { a0+b.a0, a1+b.a1, a2+b.a2 }; 
    };
};

ud create(ll x = 0, ll a = 0, ll b = 0) {
    return ud { // Calculates the sum of the powers p^k for (0 <= k <= 2) from p=a to p=b, times x
        x*(b - (a-1)),
        x*(b*(b+1)/2 - a*(a-1)/2),
        x*(b*(b+1)*(2*b+1)/6 - (a-1)*a*(2*(a-1)+1)/6)
    };
}

ud A[maxn]; // The segment tree
ll L[maxn]; // The lazy array

ll w = 0, n, m;

void update(ll x, int a, int b, int l, int r, int i);

void push(int i, int l, int r) // Pushes a lazy update down the tree
{
    if (L[i]) {
        int m = (l + r)/2;
        update(L[i], l, m, l, m, i*2);
        update(L[i], m+1, r, m+1, r, i*2 + 1);
        L[i] = 0;
    }
}

void update(ll x, int a, int b, int l, int r, int i) {
    // Adds the cost x to routes from a to b to node i spanning between points l and r
    if(b < l || a > r)
        return;

    A[i] = A[i] + create(x, a, b);

    if(a == l && b == r) { // Updates covers this range precisely, store it lazily
        L[i] += x;
        return;
    }

    // Update is a subset of this range, we need to update our children with the lazy
    // values before recursing
    push(i, l, r);

    int m = (l+r)/2;
    // Recurse to the two kids
    update(x, std::max(l, a), std::min(b, m), l, m, i*2);
    update(x, std::max(a, m+1), std::min(b, r), m+1, r, i*2+1);
}

ud query(int a, int b, int l, int r, int i) {
    // Queries the sum of powers between cities a and b for node i spanning between points l and r
    if(b < l || a > r)
        return { 0, 0, 0 };

    if(a == l && b == r)
        return A[i];

    push(i, l, r);

    int m = (l+r)/2;
    auto u = query(std::max(l, a), std::min(b, m), l, m, i*2);
    auto v = query(std::max(a, m+1), std::min(b, r), m+1, r, i*2+1);
    return u + v;
}

void init() {
    for(w = 1; w < n; w*=2);
}

int main() {
    scanf("%lld %lld", &n, &m);
    init();
    for(int i = 0; i < m; i++) {
        char s[20];
        ll a, b, d;
        scanf("%s", s);
        if(s[0] == 'c') {
            scanf("%lld %lld %lld", &a, &b, &d);
            update(d, a, b-1, 1, w, 1);
        } else {
            scanf("%lld %lld", &a, &b);
            auto ps = query(a, b-1, 1, w, 1);
            auto ans = ps.a0*(b-a*b) + ps.a1*(b+a-1) - ps.a2; // The formula given in the header
            printf("%Lf\n", ld(ans)/((b-a)*(b-a+1)/2)); // Div by the number of routes from a to b
        }
    }
}
