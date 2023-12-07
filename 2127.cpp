/* 2127. Determinant of a Graph - http://acm.timus.ru/problem.aspx?num=2127
 *
 * Strategy:
 * Utilize the sparsity of the matrix by representing each row of the matrix as a set of { index,
 * value } pairs, and use Gaussian elimination to reduce the matrix to row echelon form to calculate
 * the determinant.
 *
 * Performance:
 * Runs the tests in 0.859s using 75,360KB of memory.
 */

#include <algorithm>
#include <stdio.h>
#include <unordered_set>
#include <vector>
#include <iostream>
#include <numeric>

using ll = long long;
const ll p = 998244353;
const ll maxn = 2e5+2;

using row = std::vector<std::pair<ll, ll>>;

row H[maxn]; // The row entries, sorted by indices
std::unordered_set<ll> V[maxn]; // Nonzero indices of each column
int n, m;

int R[maxn]; // Maps a row from its container to its row index
int invR[maxn]; // Does the inverse mapping

ll mod_exp(ll b, ll e) {
    // Modular exponentiation by squaring, used for calculating inverse using FLT
    b %= p;
    ll result = 1;
    while (e > 0) {
        if (e & 1)
            result = (result*b)%p;
        b = (b*b)%p;
        e /= 2;
    }
    return result;
}

ll mod(ll a) {
    // Calculates a mod p, works for negative numbers
    while(a < 0)
        a += p*p;
    return a%p;
}

row mul(ll m, row& u, row& v, int r) {
    // Performs multiply-and-add, multiplying m with row u to row v, with merge-like iteration
    int i = 0, j = 0;
    std::vector<std::pair<ll ,ll>> out;
    while(i < u.size() && j < v.size()) {
        if(u[i].first < v[j].first) {
            // Append the multiplied value of the current index at u to row v
            if(auto s = mod(u[i].second*m); s) {
                out.push_back({ u[i].first, mod(s) });
                V[u[i].first].insert(r);
            }
            i++;
        } else if(u[i].first == v[j].first) {
            // Multiply-and-add from u to the value at the current index at row v
            if(auto s = mod(v[j].second + u[i].second*m); s)
                out.push_back({ u[i].first, mod(v[j].second + u[i].second * m) });
            else
                V[v[j].first].erase(r);
            i++, j++;
        } else {
            // No value at row u, so we end up with the value at row v
            out.push_back({ v[j].first, v[j].second });
            j++;
        }
    }
    while(i < u.size()) {
        // No more v, backfill u
        if(auto s = u[i].second*m; s) {
            out.push_back({ u[i].first, mod(s) });
            V[u[i].first].insert(r);
        }
        i++;
    }
    while(j < v.size()) {
        // No more u, backfill v
        if(auto s = v[j].second; s) {
            out.push_back({ v[j].first, mod(s) });
            V[v[j].first].insert(r);
        }
        j++;
    }

    return out;
}

ll calc() {
    // Calculates the value of the determinant
    ll ret = 1;
    for(int i = 0; i < n; i++) {
        if(V[i].empty())
            return 0;

        // Find a non-zero row at this column
        auto y = *V[i].begin();
        if(R[y] != i) {
            // Found one, swap it up to the current index to make it pivot
            std::swap(R[invR[i]], R[y]);
            invR[R[invR[i]]] = invR[i];
            invR[R[y]] = y;
            ret = mod(-ret);
        }

        auto t = H[y].front().second;
        ll inv = mod_exp(t, p - 2);

        std::vector<ll> v(V[i].begin(), V[i].end()); // The result of the row operation
        for(auto y2 : v) // Row reduce each row using the pivot row
            if(R[y2] > i)
                H[y2] = mul(mod(-inv*H[y2][0].second), H[y], H[y2], y2);

        for(auto p : H[y])
            V[p.first].erase(y);
        ret = mod(ret*H[y][0].second);
    }
    return ret;
}

int main() {
    scanf("%d %d", &n, &m);
    for(int i = 0; i < m; i++) {
        int a, b;
        scanf("%d %d", &a, &b);
        a--, b--;
        H[a].push_back({ b, 1 });
        V[a].insert(b);
        V[b].insert(a);
        H[b].push_back({ a, 1 });
    }
    std::iota(invR, invR+n, 0);
    std::iota(R, R+n, 0);

    for(int i = 0; i < n; i++)
        std::sort(H[i].begin(), H[i].end());
    printf("%lld", calc());
}
