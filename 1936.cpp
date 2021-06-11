/* 1936. Roshambo - http://acm.timus.ru/problem.aspx?num=1936
 *
 * Strategy:
 * The expected total number of rounds is the sum over p[k]/q[k] where q[k] is the probability for
 * a game with k players to resolve in one round, and p[k] is the probability to ever reach a game
 * with that number of players.
 *
 * Performance:
 * O(n^3), runs the tests in 0.015s using 300KB memory.
 */

#include <iostream>
#include <iomanip>

using ld = long double;

ld p[201], q[201];

ld C(ld n, ld k) {
    // n choose k
    ld c = 1;
    for(int i = n; i > n-k; i--)
        c *= i;
    for(int i = 1; i <= k; i++)
        c /= i;
    return c;
}

int main() {
    int n;
    std::cin >> n;

    p[n] = 1;
    for(int i = n; i >= 2; i--) {
        // The probability to resolve the game is the probability of the players showing one of at
        // most two gestures, minus the probability of them all showing exactly one gesture
        q[i] = (std::pow(2/3.0l, i) - 2*std::pow(1/3.0l, i))*3;

        // For each number of j of losing players, calculate the probability of achieving that
        // particular number and add to p[j]
        for(int j = 1; j < i; j++)
            p[j] += p[i]*C(i, j)/(std::pow(2.0l, i) - 2);
    }
    ld e = 0;

    for(int i = 2; i <= n; i++)
        e += p[i]/q[i];
    std::cout << std::setprecision(40) << e;
}
