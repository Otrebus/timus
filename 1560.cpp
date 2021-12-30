/* 1560. Elementary Symmetric Functions - http://acm.timus.ru/problem.aspx?num=1560
 *
 * Strategy:
 * Use a segment tree where each node has the five S[k]'s for the corresponding range. Combining
 * the S[k] of two ranges for each k is done by multiplying together S'[k1] + S''[k2] for every
 * k1+k2 = k, since all combinations of factors of length 3 can be formed by multiplying together
 * factors of smaller length. For example, for k = 3, we find all factors of length 3 from two
 * sequences A, B of length 3 (meaning, we find all factors of length 3 in a sequence of length 6)
 * by multiplying together factors of length 2 and 1 (and including the original length 3 factors):
 * 
 *   A[I_1]*A[I_2]*A[I_3] +
 *   (A[I_1] + A[I_2] + A[I_3]) * (B[I_1]*B[I_2] + B[I_2]*B[I_3] + B[I_1]*B[I_3]) +
 *   (A[I_1]*A[I_2] + A[I_2]*A[I_3] + A[I_1]*A[I_3]) * (B[I_1] + B[I_2] + B[I_3]) +
 *   B[I_1]*B[I_2]*B[I_3]
 * 
 * This is equivalent to multiplying S[i]*S[k-i] for k = 3 and 0 <= i <= 3. We do the equivalent
 * for all k.
 * 
 * Performance:
 * O(M log N), runs the tests in 0.703 seconds using 8,332KB memory.
 */

#include <stdio.h>
#include <algorithm>

using ll = long long;

int i, k, p, l, r;
ll S, M, P, N;

struct ans {
    // The S[k]'s
    ans() {
        std::memset(A, 0, sizeof(A));
        A[0] = 1;
    }
    ll& operator[] (int i) { return A[i]; }
    ans operator* (ans& b) { // The operation described in the header
        ans a;
        for(i = 1, a[i] = 0; i < 5; i++) 
            for(int j = 0; j <= i; j++)
                a[i] = (a[i] + (P*P + (*this)[i-j]*b[j])%P)%P;
        return a;
    }
    ll A[5];
};
ans A[1<<18];

void insert(int k, int p, int n, int L, int R) {
    // Increases the value at position k by p - n is the current position and L, R are the
    // endpoints of the range
    if(n == p)
        A[n][1] += k;
    else {
        int M = (L+R)/2;
        if(p > M) // Value is in right subrange
            insert(k, p, n*2+1, M+1, R);
        else // Value is in left subrange
            insert(k, p, n*2, L, M);
        A[n] = A[n*2]*A[n*2+1]; // After inserting into subrange, update this value
    }
}

ans query(int a, int b, int n, int L, int R) {
    // Queries A[k] for the range [a, b]
    a = std::max(a, L), b = std::min(b, R);
    int M = (L+R)/2;
    if(a == L && b == R) // We found the range
        return A[n];
    ans a1, a2;
    if(b > M) // Need to explore the right subrange
        a1 = query(a, b, n*2+1, M+1, R);
    if(a <= M) // Same but left
        a2 = query(a, b, n*2, L, M);
    return a1*a2;
}

int main() {
    scanf("%lld %lld %lld", &N, &M, &P);
    for(S = 1; S <= N*2; S *= 2); // Calculate the size of the tree

    for(int i = 0; i < N; i++)
        scanf("%lld", &A[S/2+i][1]);

    for(int i = S/2-1; i >= 1; i--) // Initialize the tree
        A[i] = A[i*2]*A[i*2+1];

    for(int i = 0; i < M; i++) {
        char s[2];
        scanf("%s", &s);
        if(s[0] == 'I') {
            scanf("%d %d", &p, &k);
            insert(k, S/2+p-1, 1, S/2, S-1);
        } else if(s[0] == 'C') {
            scanf("%d %d %d", &l, &r, &k);
            ans a = query(S/2+l-1, S/2+r-1, 1, S/2, S-1);
            for(int i = 0; i <= k; i++)
                printf("%lld ", a.A[i]);
            printf("\n");
        }
    }
}
