/* 2160. Meta-statement - http://acm.timus.ru/problem.aspx?num=2160
 *
 * Strategy:
 * Equivalent templates always have their maximum elements in the same locations. Beyond that,
 * we can redistribute numbers as we choose. After we leave the maximum number N in its original
 * place, we can choose what numbers to put in the left subtemplate in C(N, l) ways and in the
 * right subtemplate in C(N, r) ways to get an equivalent sample (where C is the n choose k
 * function), and then we recurse. To quickly find the maximum element in an array we use a sparse
 * table.
 *
 * Performance:
 * O(Nlog^2 N), runs the tests in 0.062s using 13,316KB memory.
 */

#include <stdio.h>
#include <vector>
#pragma comment(linker, "/STACK:16777216")

using ll = long long;

const int maxn = 100000;
const ll P = 1000000007;
int N, M;
ll A[maxn]; // The input values
ll F[maxn+1]; // Factorial mod P
ll E[maxn+1]; // Inverse mod P of factorial
int R[17][100000];

ll mod_pow(ll x, ll y, ll p) { // Modular exponentiation
    ll res = 1;
    for (x %= p; y > 0; y = y >> 1, x = (x*x)%p)
        if (y & 1)
            res = (res*x) % p;
    return res;
}

ll inv(ll x) { // Inverse mod P
    return mod_pow(x, P-2, P);
}

ll C(int n, int k) { // n choose k
    return (F[n]*((E[k]*E[n-k])%P))%P;
}

int mx(int a, int b) {
    return A[a] < A[b] ? a : b;
}

int min(int l, int r) { // Finds the index of the minimum element between l and r in A
    int x = r - l, i = 0;
    while (x >>= 1)
        i++;
    return mx(R[i][l], R[i][r-(1<<i)]);
}

ll count(int a, int b) { // Solves the problem recursively
    if(b == a)
        return 1;
    int p = min(a, b);
    return ((count(a, p)*C(b-a-1, p-a))%P*(count(p+1, b))%P)%P;
}

int main() {

    scanf("%d", &N);
    for(int i = 0; i < N; i++)
        scanf("%d", &A[i]);

    // Initialize the sparse table
    for(int i=0; i<N; i++)
        R[0][i] = i;
    for(int i=1; (1 << i) <= N; i++)
        for(int j=0; (j + (1 << i) - 1) < N; j++)
            R[i][j] = mx(R[i-1][j], R[i-1][j+(1<<(i-1))]);

    // Calculate the factorials and inverses
    F[0] = E[0] = 1;
    for(int i = 1; i <= maxn; i++)
        E[i] = inv(F[i] = (F[i-1]*i)%P);

    printf("%lld", count(0, N)%P);
}
