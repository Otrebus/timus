/* 1435. Financial Error - http://acm.timus.ru/problem.aspx?num=1435
 *
 * Strategy:
 * Find the first differing digit from the given sum and the actual sum and find some input number
 * that helps produce the correct sum when switching the digit (see comments below for details).
 *
 * Performance:
 * O(N), runs the tests in 0.203s using 2,952KB memory.
 */

#include <iostream>
#include <vector>

using ull = unsigned long long;

ull Z[17]; // 1, 10, 100 ..

ull d(ull n, int i) {
    // Returns the ith digit of n
    return (n/Z[i]) % 10;
}

ull swapped(ull n, int k) {
    // Returns n but with digits k and k+1 swapped
    ull x = d(n, k);
    ull y = d(n, k+1);
    return (n - y*Z[k+1] - x*Z[k] + y*Z[k] + x*Z[k+1]);
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, k = 0; // Input size, first mismatching digit of the sum vs the real sum
    ull r = 0, s = 0, z = 1; // Real sum, given sum, helper for Z

    std::cin >> n;

    std::vector<ull> v(n); // The input
    std::vector<ull> T(n); // The position of the largest digit

    for(int i = 0; i < 17; i++, z*= 10)
        Z[i] = z;

    for(int i = 0; i < n; i++) {
        std::cin >> v[i];
        r += v[i]; // Add to the real sum
        for(ull r = v[i]/10; r; r /= 10) // Calculate the pos of the largest digit
            T[i]++;
    }
    std::cin >> s;

    if(r == s) {
        std::cout << "Input has no error.";
        return 0;
    }
    for(int i = 0; i < 16; i++) {
        // Find the first bad differing digit
        if(d(s, i) != d(r, i)) {
            k = i;
            break;
        }
    }

    for(int i = 0; i < n; i++) {
        // For each input number, try swapping k and k+1 but only if k isn't the leading digit
        // and if it's not the second leading digit and 0
        if(v[i] > Z[k+1] && (T[i] != k+1 || d(v[i], k))) {
            auto sw = swapped(v[i], k);
            auto t = r - v[i] + sw; // Sum with the swapped number
            if(t == s) {
                std::cout << "Error in record #" << (i+1) << ". ";
                std::cout << "Correct number is: " << sw << ".";
                return 0;
            }
        }
    }
    std::cout << "Unrecoverable error.";
}
