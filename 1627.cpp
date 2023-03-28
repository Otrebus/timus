/* 1627. Join - http://acm.timus.ru/problem.aspx?num=1627
 *
 * Strategy:
 * Use Kirchhoff's theorem to count the number of spanning subtrees of the graph.
 *
 * Performance:
 * O((mn)^3 log(mn)), Runs the tests in 0.015s using 216KB memory.
 */

#include <stdio.h>
#include <algorithm>

typedef long long ll;
const ll p = 1e9;
const int maxn = 81;
ll A[maxn][maxn]; // The Laplacian matrix
int B[maxn][maxn]; // The vertex number of each room
char S[9][10]; // The input
int n, m, r = -1;

ll calc() { // Calculates the determinant of the Laplacian
    ll ret = 1;
    for(int i = 1; i <= r; i++) {
        for(int j = i + 1; j <= r; j++) {
            // We can't calculate the modular inverse, but we can whittle down the pivot column:
            while(A[j][i]) {
                ll t = A[i][i]/A[j][i];
                for(int k = i; k <= r; k++)
                    A[i][k] = (p*p + A[i][k] - t*A[j][k])%p;
                std::swap(A[i], A[j]);
                ret = -ret;
            }
        }
        ret = (p*p + ret*A[i][i])%p;
    }
    return ret;
}

void add(int i, int j, int i2, int j2) { // Connects two rooms in the graph
    if(i2 < 0 || i2 >= n || j2 < 0 || j2 >= m)
        return;
    if(S[i2][j2] == '.') {
        int a = B[i][j], b = B[i2][j2];
        A[a][a]++;
        A[a][b] = A[b][a] = -1;
    }
}

int main() {
    scanf("%d %d", &n, &m);
    for(int i = 0; i < n; i++) {
        scanf("%s", S[i]);
        for(int j = 0; j < m; j++)
            B[i][j] = (r += S[i][j] != '*');
    }
    for(int i = 0; i < n; i++)
        for(int j = 0; j < m; j++)
            if(S[i][j] == '.')
                for(int p : { -1, 1 })
                    add(i, j, i+p, j), add(i, j, i, j+p);
    printf("%d", calc());
}
