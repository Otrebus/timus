/* 1593. Square Country. Version 2 - http://acm.timus.ru/problem.aspx?num=1593
 *
 * Strategy:
 * Any positive integer can be written at most as the sum of four squares. Testing for the case
 * of one square is trivial, for two squares we use the Sum of Two Squares theorem, and Legendre's
 * Three-square theorem rules out the case for three squares.
 *
 * Performance:
 * O(sqrt N), runs the tests in 0.187s using 396KB memory.
 */

#include <iostream>
#include <cmath>

using ll = long long;
using ld = long double;

int solve3(ll n) {
    // Checks Legendre's condition and assumes we ruled out 1 or 2 squares
    do {
        if(n % 8 == 7)
            return 4;
    } while (n % 4 == 0 && (n /= 4));
    return 3;
}

int solve(ll n) {
    ll m = n, k = 0;

    auto st = (ll) std::sqrt((ld) n);
    if(st*st == n) // The simplest case
        return 1;

    while(n % 2 == 0)
        n /= 2; // Eliminate the even factors so we can increment by 2 next
    // Decompose into prime factors to run by the sum-of-two-squares theorem
    for(int p = 3; p < st; p += 2, k = 0) {
        while(n % p == 0)
            k++, n /= p;
        if(p % 4 == 3 && k % 2) // A factor p^(2n+1) where p = 3 (mod 4) rules out two squares
            return solve3(m);
    }
    if(n % 4 != 3) // In case n was prime itself, this is the only prime factor
        return 2;
    return solve3(m); // At this point we definitely ruled out two squares
}

int main() {
    ll n;
    std::cin >> n;
    std::cout << solve(n);
}
