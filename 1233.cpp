/* 1233. Amusing Numbers - http://acm.timus.ru/problem.aspx?num=1233
 *
 * Strategy:
 * Calculate the number of numbers we necessarily have to the left of the given number K, and then
 * increase the number of digits of N in order to add more numbers to the left of K to reach the
 * necessary M. For example, for K = 2 and M = 43, we necessarily have 1 before 2, but beyond this
 * we then need the full range of numbers 10 through 19, and the remaining 31 of the 42 (=43-1)
 * numbers we get from the beginning of the range 100 through 199, specifically 100 to 130, leading
 * to the answer of N = 130.
 *
 * Performance:
 * O(log M), runs the tests in 0.015s using 392KB memory.
 */

#include <iostream>

using ll = long long;

ll p[20];

ll z(ll k) { // Returns the place value of the least nonzero digit of k
    int i = 0;
    while(k /= 10)
        i++;
    return p[i];
}

ll calc(ll K, ll M) {

    for(ll i = 0, w = 1; i < 19; i++) // Calculate powers of 10
        p[i] = w, w *= 10ll;

    ll n = 0;
    for(ll k = K; k; k /= 10) // Add necessary numbers
        n += 1 + (k - z(k));

    if(M < n)
        return 0;
    M -= n;
    if(M) { // Increase N to pad the necessary number left of K
        for(int i = 1; ; i++) {
            ll l = p[i]*(K-z(K)); // The number of numbers in the full range (e.g. 100-199 in the
            if(!l)                // header comment)
                return 0;
            if(M <= l) // The number we need to add is less than the full range, find N among them
                return M-1 + z(K)*p[i];
            M -= l;
        }
    } else
        return K;
}

int main() {
    ll K, M;
    std::cin >> K >> M;
    std::cout << calc(K, M);
}
