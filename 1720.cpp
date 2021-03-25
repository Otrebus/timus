/* 1720. Summit Online Judge - http://acm.timus.ru/problem.aspx?num=1720
 *
 * Strategy:
 * For some interval of numbers [x, x+w], we can keep track of all possible ways of summing these
 * numbers up by iteratively adding the interval endpoints like [2x, 2(x+w)], [3x, 3(x+w)],
 * ..., [kx, k(x+w)]. We focus on the empty space between the intervals and their size, equal to
 * x - kw - 1. We can then form a function that calculates how many empty spaces form between
 * some interval [0, t] and use it to calculate the sum of the empty spaces in the interval [l, r].
 *
 * Performance:
 * Constant, runs the tests in 0.015s using 272KB memory.
 */

#include <iostream>

using ll = long long;

ll f(ll t, ll x, ll y) {
    // Calculates the number of empty spaces from x onwards in [0, t] when the interval [x, y] is
    // added to itself
    ll w = y-x;
    if(!w) // We handle the case of a singleton interval separately
        return t < x ? 0 : t-t/x-x+1;
    // k is the number of intervals we fit up to t, bounded by x-kw=0 (where we stop getting new
    // empty spaces)
    ll k = std::min(t/y, (x-1)/w);
    // Integrate (use the summation formula) for x - kw - 1.
    ll s = k*x - w*(k*(k+1))/2 - k;

    if(k) // Account for t splitting an empty space
        s -= std::max((k+1)*x - t - 1, 0ll);
    return s;
}

int main() {
    ll x, y, l, r;
    std::cin >> x >> y >> l >> r;

    auto res = f(r, x, y) - f(l-1, x, y); // The size of the empty space between [l, r]
    // Remove this from [l, r], compensate for when x starts within the interval
    std::cout << (std::max(0ll, r-std::max(l, x) + 1)) - res;
}
