/* 1148. Building Towers - http://acm.timus.ru/problem.aspx?num=1148
 *
 * Strategy:
 * We use the recurrence A[n][h][m] = A[n-m][h-1][m-1] + A[n-m][h-1][m+1], meaning the number of
 * towers with m bricks on the lowest level and height h is the sum of the number of valid towers of
 * height h-1 on top of that bottom level, and use straightforward dfs and memoization to return
 * the necessary values. In order to satisfy the memory requirements, we use a flat array A to
 * memoize the values, and an array B[h][m] to help index into the array, which indicates the
 * highest relevant n for any given h and m (cumulatively), or, the number of bricks at most over
 * towers of parameters h and m (beyond which greater n makes no difference) (again, cumulatively).
 * We also only populate that array with only roughly every 6th value, meaning our dfs has to do
 * some duplicated work.
 *
 * Performance:
 * Runs the tests in 0.031s using 2,748KB memory.
 */

#include <vector>
#include <stdio.h>
#include <array>

using ll = long long;

ll A[329822]; // The flat memoization array
int B[72][61]; // The (cumulative) highest possible n for every h and m, used to index into A

int N, H, M;
ll k;

std::vector<int> v; // The number of bricks for each level of the kth tower

int clamp(int n, int h, int m) {
    // Limits n to the lowest necessary for towers of parameters h and m
    return std::min(n, m*h + (h-1)*h/2); // Tower of base m and height h has at most this many brix
}

ll get(int n, int h, int m) {
    // Gets the number of towers of parameters n, h and m
    if(m % 2 == 0 && h % 3 == 0) // Save space
        return A[B[h][m]+clamp(n, h, m)];
    return -1;
}

ll set(int n, int h, int m, ll r) {
    // Sets the number of towers of parameters n, h, m
    if(m % 2 == 0 && h % 3 == 0)
        return A[B[h][m]+clamp(n, h, m)] = r;
    return r;
}

ll count(int n, int h, int m) {
    // Counts the number of different towers with parameters n, h and m
    if(h == 1 || !m || n < 0)
        return m && m <= n;

    ll ret = get(n, h, m); // Already memoized
    if(ret != -1)
        return ret;

    // The recurrence mentioned in the header
    ret = count(n-m, h-1, m-1) + count(n-m, h-1, m+1);
    return set(n, h, m, ret);
}

void dfs(ll k, int n, int h, int m) {
    // Produces the number of bricks, layer by layer, to find the kth tower
    v.push_back(m); // By definition
    if(h == 1)
        return;
    ll d = count(n-m, h-1, m-1); // The number of towers on top with lowest row of m-1 bricks
    if(k <= d) // There's more than k towers starting with m-1 bricks on top of us, recurse there
        dfs(k, n-m, h-1, m-1);
    else // Else recurse to m+1 in order to get to the kth tower
        dfs(k-d, n-m, h-1, m+1);
}

int main() {
    scanf("%d %d %d", &N, &H, &M);
    int sum = 0;
    std::memset(A, 0xff, sizeof(A));

    // Calculate the B matrix
    for(int h = 0; h <= 61; h++) {
        for(int m = 0; m <= 72; m++) {
            if(m <= (60-h) + 10 && m % 2 == 0 && h % 3 == 0) {
                B[h][m] = sum;
                sum += 1 + m*h + (h-1)*h/2; // A needs up to this index for values of n
            }
        }
    }

    printf("%lld\n", count(N, H, M));
    while(true) {
        scanf("%lld", &k);
        if(k == -1)
            break;

        v.clear();
        dfs(k, N, H, M);

        for(auto x : v)
            printf("%d ", x);
        printf("\n");
    }
}
