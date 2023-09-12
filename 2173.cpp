/* 2173. Meta-statement 3 - http://acm.timus.ru/problem.aspx?num=2173
 *
 * Strategy:
 * Dynamic programming over P[k][n] where k is the number of problems that are assigned a single
 * sponsor, and n is the current sponsor. For the transition DP step, for each k, we distribute
 * the number of wishes of the current sponsor across the k single-assigned problems and the
 * non-assigned problems and multiply the number of combinations of doing so (see comments).
 *
 * Performance:
 * O(M^2+NM), runs the tests in 0.093s using 196KB memory.
 */

#include <stdio.h>
#include <vector>

using ll = long long;

const int maxn = 1000, maxm = 2000;
const ll P = 1000000007;
int N, M;
ll S[maxn+1]; // The wishes of each sponsor
ll U[maxn+1]; // The prefix sum of the wishes
ll A[maxm+1][2]; // The DP matrix
ll F[maxm+1]; // Factorials mod m
ll E[maxm+1]; // Inverses of the above

ll mod_pow(ll x, ll y, ll p) { // Modular pow function, used for calculating inverses
    ll res = 1;
    for (x %= p; y > 0; y = y >> 1, x = (x*x)%p)
        if (y & 1)
            res = (res * x) % p;
    return res;
}

ll inv(ll x) { // Modular inverse
    return mod_pow(x, P-2, P);
}

ll C(int n, int k) { // n choose k
    return (F[n]*((E[k]*E[n-k])%P))%P;
}

int main() {
    scanf("%d %d", &N, &M);
    for(int i = 1; i <= N; i++) {
        scanf("%lld", &S[i]);
        U[i] = (U[i-1] + S[i])%P;
    }

    // Calculate factorials and their inverses
    F[0] = E[0] = A[0][0] = 1;
    for(int i = 1; i <= M+1; i++)
        E[i] = inv(F[i] = (F[i-1]*i)%P);

    for(ll n = 1; n <= N; n++) {
        for(int i = 0; i <= M; i++)
            A[i][n%2] = 0;

        for(ll k = 0; k <= std::min((ll)M, U[n-1]); k++) { // Number of singly assigned problems
            auto l = M - (k+U[n-1])/2; // Number of empty problems
            // Number of the k half-assigned problems that we assign to, "completing" them
            for(int j = std::max(0ll, S[n]-l); j <= std::min(k, S[n]); j++) {
                auto& T = A[k+S[n]-2*j][n%2]; // We end up with k+S[n]-2*j half-assigned problems
                auto a = A[k][(n-1)%2]; // The number of k half-assigned problems up to now
                auto b = C(k, j); // Number of ways to assign j wishes to the k half-problems
                auto c = C(l, S[n]-j); // Number of ways to assign the rest of the wishes
                T = (T + (a*b)%P*c)%P;
            }
        }
    }
    ll sum = 0;
    for(int i = 0; i <= M; i++)
        sum = (sum + A[i][N%2])%P;
    printf("%lld", sum);
}
