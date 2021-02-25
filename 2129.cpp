/* 2129. Mortgage in Far Away Kingdom - http://acm.timus.ru/problem.aspx?num=2129
 *
 * Strategy:
 * Find the base-k representation of the given number, and start "splitting" higher bases (coins)
 * into smaller ones. For example with the input 12 3 4 we have 12 = 0*3^0 + 1*3^1 + 1*3^2, which
 * represents the number with the fewest number of coins possible. We can then add two coins
 * by "splitting" the 9^1 into three 3^1's and use the total four 3^1's, or we can skip splitting
 * the 3^2, and split the 3^1 into three 3^0's. In general this is a recursive function, where we
 * can memoize the input (see the solve function below).
 *
 * Performance:
 * O(t*l^2*log n), runs the tests in 1.203s using 8,460KB memory.
 */

#include <iostream>
#include <cstring>

using ll = long long;
const ll mod = 1000000007;

ll k;
ll pw[101];
ll C[101][101][101];

long long solve(int i, ll b, ll l) {
    // The number of ways to split the number with base-k representation given by pw
    // from position i down, given that we have b extra bases to split at position i
    // and l extra bases we can generate in total

    b = std::min(b, 100ll);
    if(!i || l < 0)
        return l == 0;
    if(C[i][b][l] > -1)
        return C[i][b][l];
    ll sum = 0;

    // We loop over the number of times we can split the base at this location and recurse
    for(int x = 0; b + pw[i] >= x && l - x*(k-1) >= 0; x++)
        sum = (sum % mod + solve(i-1, x*k, l-x*(k-1)) % mod) % mod;
    C[i][b][l] = sum;
    return sum;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int N;
    ll n, l;

    std::cin >> N;
    while(N--) {
        std::cin >> n >> k >> l;

        std::memset(C, -1, sizeof(C));
        std::memset(pw, 0, sizeof(pw));

        int i = 0;
        ll m = n, s = 0;
        // Calculate the base-k representation of the number
        for(i = 0; m; i++) {
            pw[i] = m % k;
            s += pw[i];
            m /= k;
        }

        std::cout << solve(i, 0, l-s) << std::endl;
    }
}
