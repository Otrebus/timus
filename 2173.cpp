/* 2173. Meta-statement 3 - http://acm.timus.ru/problem.aspx?num=2173
 *
 * Strategy:
 * Dynamic programming over P[k][n] where k is the number of problems that are assigned a single
 * sponsor, and n is the current sponsor. For the transition DP step we distribute the number of
 * wishes of the current sponsor across the k single-assigned problems and the non-assigned
 * problems and multiply the number of combinations of doing so.
 *
 * Performance:
 * O(M^2+NM), runs the tests in 0.062s using 15,996KB memory.
 */

#include <stdio.h>
#include <vector>
#include <algorithm>

using namespace std;

using ll = long long;

const int maxn = 1000, maxm = 2000;
const ll mod = 1000000007;
int N, M;
ll S[maxn+1]; // The wishes of each sponsor
ll U[maxn+1]; // The prefix sum of the wishes
ll A[maxm+1][2]; // The DP matrix
ll P[maxm+1][maxm+1]; // Pascal's triangle

ll C(int n, int k) {
    return P[n][std::min(k, n-k)];
}

int main() {
    P[0][0] = 1;
    for(int m = 1; m <= maxm; m++) { // Build Pascal's triangle
        P[m][0] = 1;
        for(int k = 1; k <= m/2; k++)
            P[m][k] = (P[m-1][k-1] + P[m-1][std::min(k, m-1-k)])%mod;
    }
    scanf("%d %d", &N, &M);
    for(int i = 1; i <= N; i++) {
        scanf("%lld", &S[i]);
        U[i] = (U[i-1] + S[i])%mod;
    }

    A[0][0] = 1;
    for(ll n = 1; n <= N; n++) {
        for(int i = 0; i <= M; i++)
            A[i][n%2] = 0;
         // Number of singly assigned problems = k
        for(ll k = 0; k <= std::min((ll)M, U[n-1]); k++) {
            if(U[n-1]%2 == k%2) {
                // Number of the k problems that we assign up to double = j
                for(int j = std::max(0ll, (U[n-1]+k-2*M)/2+S[n]); j <= std::min(k, S[n]); j++) {
                    auto l = M - (k+U[n-1])/2; // Number of empty problems
                    auto& T = A[k-j+S[n]-j][n%2];
                    T = (T + ((A[k][(n-1)%2]*C(k, j))%mod*C(l, S[n]-j))%mod)%mod;
                }
            }
        }
    }
    ll sum = 0;
    for(int i = 0; i <= M; i++)
        sum = (sum + A[i][N%2])%mod;
    printf("%lld", sum);
}
