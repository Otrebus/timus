/* 1903. Unidentified Ships - http://acm.timus.ru/problem.aspx?num=1903
 *
 * Strategy:
 * Combinatorics.
 *
 * Performance:
 * O(n^2), runs the test suite in 0.109s using 40,740KB memory.
 */

#include <iostream>
#include <algorithm>
#include <vector>

const int maxn = 5000;
const long long mod = 1000000007;

int A[maxn+1][(maxn+1)/2+1], s[3];

long long C(int n, int k)
{
    return k > n ? 0 : A[n][std::min(k, n-k)];
}

int main()
{
    A[0][0] = 1;
    for(int n = 1; n <= maxn; n++) // Build Pascals triangle
    {
        A[n][0] = 1;
        for(int k = 1; k <= n/2; k++)
            A[n][k] = (A[n-1][k-1] + A[n-1][std::min(k, n-1-k)])%mod;
    }

    int n, t, k, x;
    std::cin >> n >> t;
    std::vector<int> input;
    for(int i = 1; i <= n; i++)
    {
        int x;
        std::cin >> x;
        input.push_back(x);
    }
    std::cin >> k >> x;
    for(auto c : input) // Count number of ships with id greater, equal and lower than the id:d one
        s[(c > input[k-1]) - (c < input[k-1]) + 1]++;
    long long sum = 0;
    for(int m = 1; m <= std::min(s[1], t); m++) // Pick any number of ships with id equal
        for(int p = std::max(0, x-m); p <= std::min( { x-1, s[0] }); p++) // Then lower
            sum = (sum + C(s[0], p)*C(s[1]-1, m-1) % mod*C(s[2], t-(m+p)) % mod) % mod; // And count
    std::cout << sum;
    std::system("pause");
}