/* 1518. Jedi Riddle 3 - http://acm.timus.ru/problem.aspx?num=1518
 *
 * Strategy:
 * The formula described is a linear transformation of a vector of length N (think of the vector as
 * the current sliding window of the K's), so we compose the transformation matrix using
 * exponentiation by squaring.
 *
 * Performance:
 * O(N^3log X), runs the tests in 0.812s using 280KB memory.
 */

#include <memory>
#include <vector>

using ll = long long;
ll N, X, Y;
ll K[100], C[100];

struct matrix { // A matrix with modulo multiplication
    std::vector<std::vector<ll>> A;
    matrix(int n) { A.resize(N, std::vector<ll>(N, n)); }
    matrix() {}
    matrix operator* (const matrix& v) {
        matrix m(0);
        for(int i = 0; i < N; i++)
            for(int j = 0; j < N; j++)
                for(int k = 0; k < N; k++)
                    m.A[i][j] = (m.A[i][j] + A[i][k]*v.A[k][j])%Y;
        return m;
    }
};

matrix exp(matrix& A, ll n) {
    // Returns A^n
    if(n == 1)
        return A;
    if(n%2) {
        return A*exp(A, n-1);
    } else {
        auto b = exp(A, n/2);
        return b*b;
    }
}

int main() {
    scanf("%lld %lld %lld", &N, &X, &Y);

    matrix m(0);

    for(int i = 0; i < N; i++)
        scanf("%lld", &K[i]);

    // Construct the matrix
    for(int i = 0; i < N; i++)
        scanf("%lld", &m.A[N-1][i]);
    for(int i = 0; i < N-1; i++)
        m.A[i][i+1] = 1;
    
    m = exp(m, X-N);

    // Do the final matrix/vector multiplication
    ll ans = 0;
    for(int j = 0; j < N; j++)
        ans = (ans + m.A[N-1][j]*K[j])%Y;
    printf("%lld", ans);
}
