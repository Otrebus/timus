/* 1608. Lucky Tickets 2008 - http://acm.timus.ru/problem.aspx?num=1608
 *
 * Strategy:
 * We consider the problem of counting the lucky tickets from 1 to N, which we solve recursively.
 * For some number 543519, we count the number of tickets up to 99999, then the number of non-
 * palindromes with 43250 or less in the middle position (meaning we accept leading zeros),
 * multiplied by nine which is the number of rightmost digits that don't clash with the leftmost
 * 4 of the outer number, plus the number of palindromes in the middle position less than 99999
 * multiplied by 3*9 (which is the number of leftmost digits that don't clash with the rightmost
 * digit, multiplied by the number of rightmost digits that don't clash with the leftmost one). Etc.
 *
 * Performance:
 * O(log b), runs the tests in 0.015s using 276KB memory.
 */

#include <iostream>

using ll = long long;

ll w(ll a) {
    // The closest power of 10 not greater than a
    ll x = 1;
    while(a/x > 9)
        x *= 10;
    return x;
}


bool unbalanced(ll m, ll Wi) {
    // Returns true if the number is not a palindrome
    ll b = 1;
    bool ba = true;
    for(ll t = Wi; t > b; t /= 10, b *= 10) {
        ba &= ((m/t)%10 != (m/b)%10);
    }
    return ba;
}

ll mid(ll m, ll W) {
    // Returns the number with leftmost and rightmost digits removed
    ll D = m/W;
    return (m - W*D)/10;
}

ll decap(ll& m, ll Wi) {
    // If we decrement m until the lowest digit is 0, returns the number of times the
    // rightmost digit doesn't match the leftmost
    ll d = m%10, D = m/Wi;
    m -= d + 1;
    return (d + 1 - (D <= d));
}

ll solveMiddle(ll m, ll Wi) {
    // Returns the number of nonpalindromes among numbers less than m, all with width Wi
    // meaning we count leading zeroes
    bool ba = !Wi || unbalanced(mid(m, Wi), Wi/100);

    if(m < 10) 
        return ba ? m + (Wi <= 1) : 0;

    ll r = ba ? decap(m, Wi) : 0;

    auto a = 9*(solveMiddle(mid(m, Wi), Wi/100));
    auto b = (m/Wi)*9*(solveMiddle(Wi/10-1, Wi/100));
    return a + b + r;
}

ll solve(ll m) {
    // Returns the number of lucky tickets (nonpalindromes) below m
    ll M = m, Wi = w(m);

    bool ba = unbalanced(mid(m, Wi), Wi/100);

    if(m < 10)
        return ba ? m + (Wi <= 1) : 0;

    ll r = ba ? decap(m, Wi) : 0;

    if(w(m) != w(M))
        return solve(m) + r;

    ll a = m/Wi ? solve(w(M)-1) : 0LL;        
    ll b = 9*solveMiddle(mid(m, Wi), Wi/100);
    ll c = (m/w(M)-1)*9*(solveMiddle(w(M)/10-1, Wi/100));
    return a + b + c + r;
}

int main() {
    ll x, y;
    std::cin >> x >> y;
    std::cout << (solve(y) - solve(x-1));
}
