/* 2152. Tax Fees - http://acm.timus.ru/problem.aspx?num=2152
 *
 * Strategy:
 * Straightforward (find the biggest tax increase for each package, use the k most tax-increased
 * packages) but since we're dealing with high precision requirements we have use integer math
 * whenever we can.
 *
 * Performance:
 * Linearithmic in n, runs the tests in 0.515s using 12,696KB memory.
 */

#include <vector>
#include <algorithm>
#include <iomanip>

using ll = long long;
using ld = long double;

struct package {
    ll w, p, nw, np; // Weight cost, new weight, cost
    int i; // Original index
} p[200000];

ll n, k, mxw, mxp, a, b;

ll replace(ll n, ll z) {
    // Replaces the given digit, valued z, with a 9
    ll d = (n/z)%10;
    return n - (z*d) + (z*9);
}

ll calcfp(ll p) {
    // Calculates the tax of some cost, times 100
    if(p > mxp)
        return (p-mxp)*b;
    return 0;
}

ll calcfw(ll w) {
    // Calculates the tax of some weight
    if(w > mxw)
        return (w-mxw)*a;
    return 0;
}

bool cmp(package& a, package& b) {
    // Comparison function of packages with respect to largest tax increase (using integers only)

    // The tax increases with respect to cost and weight for the packages
    ll daw = calcfw(a.nw) - calcfw(a.w), dbw = calcfw(b.nw) - calcfw(b.w);
    ll dap = calcfp(a.np) - calcfp(a.p), dbp = calcfp(b.np) - calcfp(b.p);

    if(a.nw != a.w) { // If a doesn't change weight
        if(b.nw != b.w) // If b doesn't either
            return daw < dbw; // Compare weights
        if(daw > 100000000000ll) // Can't possibly beat this
            return false;
        // Compare weight vs tax increase, account for the fact we're multiplying %'s by 100
        return 100*daw < dbp;
    } else { // Similar, see above
        if(b.np != b.p)
            return dap < dbp;

        if(dbw > 100000000000ll)
            return true;
        return dap < 100*dbw;
    }
    return false;
}


int main() {

    scanf("%lld %lld %lld %lld %lld %lld", &n, &k, &mxw, &mxp, &a, &b);

    for(int i = 0; i < n; i++) {
        scanf("%lld %lld", &p[i].w, &p[i].p);

        p[i].i = i;
        ll maxw = 0, maxp = 0;

        // Calculate the most taxes we can get out of this package
        for(ll j = 1; j <= 1000000000 && p[i].w >= j; j*= 10) { // Find best digit to modify weight
            ll x = replace(p[i].w, j);
            if(x != p[i].w)
                maxw = x;
        }

        for(ll j = 1; j <= 1000000000 && p[i].p >= j; j*= 10) { // Same with cost
            ll x = replace(p[i].p, j);
            if(x != p[i].p)
                maxp = x;
        }

        p[i].nw = p[i].w;
        p[i].np = p[i].p;

        // Form two packages (weight vs cost) and use our comparison function
        package a { p[i].w, p[i].p, p[i].w, maxp };
        package b { p[i].w, p[i].p, maxw, p[i].p };
        if(cmp(a, b))
            p[i].nw = maxw;
        else
            p[i].np = maxp;
    }
    std::sort(p, p+n, cmp); // Sort all of our packages

    for(int i = n-k; i < n; i++) { // Use the k most increased prices
        p[i].w = p[i].nw;
        p[i].p = p[i].np;
    }

    // Since we have to be super exact we use fixed point arithmetic to print out the results
    ll c[3] = { 0, 0, 0 };
    ll d = 1000000000000000000; // Size of our fixed point "digits" above the decimals
    for(int i = 0; i < n; i++) {
        ll t = calcfp(p[i].p), w = calcfw(p[i].w);

        // Standard long addition from decimals up to the biggest digit
        c[0] = (c[0] + t%100);
        c[1] += c[0]/100 + t/100 + w;
        c[0] %= 100;
        c[2] += c[1]/d;
        c[1] %= d;
    }
    if(c[2]) // If we used the highest digit
        printf("%lld", c[2]);
    printf(c[2] ? "%018lld" : "%lld", c[1]); // Print second digit, pad with 0's if necessary
    printf(".%02lld\n", c[0]); // Print the decimals

    // Put the packages back in their original order and print
    std::sort(p, p+n, [] (auto& a, auto& b) { return a.i < b.i; } );
    for(int i = 0; i < n; i++)
        printf("%lld %lld\n", p[i].w, p[i].p);
}
