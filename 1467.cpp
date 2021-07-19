/* 1467. Sum of Degrees - http://acm.timus.ru/problem.aspx?num=1467
 *
 * Strategy:
 * If we denote the sum of powers 1^k + ... + n^k as S(k, n), consider the arrangement
 * 
 *                           n^k
 *                      .
 *                      .
 *               3^k + ... + n^k
 *         2^k + 3^k + ... + n^k
 *   1^k + 2^k + 3^k + ... + n^k.
 *
 * We have
 * 
 *   1*1^k + 2*2^k + 3*3^k + ... + n*n^k = 1^(k+1) + 2^(k+1) + 3^(k+1) + ... + n^(k+1) = S(k+1, n).
 * 
 * We can reach an alternative formulation if we fill n+1 rows of 1^k + .. + n^k (instead of just
 * the triangle above), and subtract the rest:
 * 
 *   S(k+1, n) = (n + 1)*S(n, k) - (1^k + (1^k + 2^k) + (1^k + 2^k + 3^k) + ... (1^k + ... + n^k))
 * 
 * The subtrahend above is the sum of S(k, i) from i = 1 to n, so we have the recurrence
 * 
 *   S(k+1, n) = (n + 1)*S(n, k) - sum_{i=1}^n S(k, i)
 * 
 * For example, for k=1 we get the formula
 * 
 *   S(2, n) = (n + 1)*S(n, 1)    - sum_{i=1}^n (S(1, i))
 *           = (n + 1)*n(n + 1)/2 - sum_{i=1)^n (i(1 + i)/2)
 *           = n(n + 1)^2/2       - (S(1, n)/2 + S(2, n)/2)
 * 
 * We then use our previously calculated value for S(1, n) to solve for the expression of S(2, n).
 * 
 * Performance:
 * O(n^3), runs the tests in 0.015s using 196KB memory.
 */

#include <stdio.h>
#include <vector>

using ll = long long;

ll gcd(ll a, ll b) { return b ? gcd(b, a%b) : a; }

struct frac {
    // Represents the fraction a/b and the standard way of adding/multiplying them - we
    // always reduce to the least common terms after each operation
    ll a, b;
    frac(ll a=0, ll b=1) : a(a), b(b) { }
    frac normalize() {
        int g = gcd(a, b);
        a /= g, b /= g;
        if(b < 0)
            a *= -1, b *= -1;
        return { a, b };
    }
    frac operator+ (const frac& x) { return frac { a*x.b + b*x.a, b*x.b }.normalize(); }
    frac operator- (const frac& x) { return frac { a*x.b - b*x.a, b*x.b }.normalize(); }
    frac operator* (const frac& x) { return frac { a*x.a, b*x.b }.normalize(); }
    frac inv() { return frac { b, a }; }
};

struct poly {
    // Represents the coefficients of a sum of powers v[0] + v[1]*p^1 + v[2]*p^2 + ...
    frac v[32];
    poly() { std::fill(v, v+32, frac(0, 1)); };
    poly(std::vector<frac> p) { std::copy(p.begin(), p.end(), v); } 
    poly operator+ (poly b) {
        poly out;
        for(int i = 0; i < 32; i++)
            out.v[i] = v[i] + b.v[i];
        return out;
    }
    poly operator- (poly b) {
        for(int i = 0; i < 32; i++)
            b.v[i] = v[i] - b.v[i];
        return b;
    }
    poly operator* (frac b) {
        poly out;
        for(int i = 0; i < 32; i++)
            out.v[i] = v[i] * b;
        return out;
    }
    poly mul() { // Multiplies by p
        poly p;
        for(int i = 1; i < 32; i++)
            p.v[i] = v[i-1];
        return p;
    }
};

int main() {
    int n;
    scanf("%d", &n);

    // The coefficients of S(k, n). For k = 0, 1^0 + 2^0 + ..  + n^0 = n = 0*n^0 + 1*n
    std::vector<poly> S = {{{ frac(0), frac(1) }}};
    for(int k = 1; k <= n; k++) {
        poly y; // The subtrahend mentioned in the header
        for(int i = 0; i < k; i++)
            y = y - S[i]*S[k-1].v[i]; // Use previously calculated values
        auto x = frac(1) + S[k-1].v[k]; // We move over the p^k term to the left side
        S.push_back((S[k-1] + S[k-1].mul() + y)*x.inv());
    }
    for(int i = n+1; i >= 0; i--)
        printf("%lld/%lld ", S[n].v[i].a, S[n].v[i].b);
}
