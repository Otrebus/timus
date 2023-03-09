/* 1926. The Tournament of Intelligences - http://acm.timus.ru/problem.aspx?num=1926
 *
 * Strategy:
 * Maintain the remainders n mod p_i for each i with every update operation. We then apply the
 * Chinese Remainder Theorem where in the simultaneous system the modulos are the different prime
 * numbers p_i, to calculate the query n mod c_i.
 *
 * Performance:
 * O(m), runs the tests in 0.093s using 11,984KB memory.
 */

#include <stdio.h>
#include <tuple>
#include <cstring>

using ll = long long;

using namespace std;
ll p[] = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47 };
const ll P = 15;
ll Q[15]; // The composition of the current query
ll A[100000][15]; // The moduli of 10^k for each prime
char S[100001]; // The input string
ll B[15]; // The modulus of the number for each prime
ll a, b;
int N, t;

ll modInverse(ll a, ll m) { // Finds the multiplicative inverse of a mod m
    a %= m;
    for (int x = 1; x < m; x++)
        if ((a * x) % m == 1)
            return x;
    return -1;
}

ll multi(ll a, ll b, ll mod) { // Multiplies (a*b)%mod using addition (because of overflow)
    ll sum = 0;
    while(b--)
        sum = (sum + a)%mod;
    return sum;
}

ll calc() {
    // Calculates the remainder of n modulo c_i with the help of the CRT
    ll M = 1;
    for (ll i = 0; i < P; i++)
        if(Q[i])
            M *= p[i];

    ll x = 0;
    for (ll i = 0; i < P; i++) {
        if(Q[i]) {
            ll q = B[i];
            for(int j = 0; j < P; j++)
                if(Q[j] && i != j)
                    q = (q*p[j])%M;
            q = multi(q, modInverse(M/p[i], p[i]), M);
            x = (q + x)%M;
        }   
    }

    return (x%M + M)%M;
}

int main() {
    scanf("%s", S);
    ll L = strlen(S);

    for(int i = 0; i < L; i++) { // Precalculate the remainders of base 10's and the big number (S)
        for(int j = 0; j < P; j++) {
            A[i][j] = (i > 0 ? (A[i-1][j]*10) : 1)%p[j];
            B[j] = (B[j] + ((S[L-i-1]-'0')*A[i][j]))%p[j];
        }
    }

    scanf("%d", &N);
    while(N--) {
        scanf("%d", &t);
        if(t == 0) {
            scanf("%lld", &a);
            for(int i = 0; i < P; i++)
                Q[i] = a%p[i] == 0;
            printf("%lld\n", calc());
        } else {
            scanf("%lld %lld", &a, &b);
            for(int j = 0; j < P; j++) // Update the remainders modulo the primes of the big number
                B[j] = (B[j] + (A[a-1][j])*(b-(S[L-a]-'0')))%p[j];
            S[L-a] = '0' + b;
        }
    }
}
