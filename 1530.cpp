/* 1530. Ones and Zeroes - http://acm.timus.ru/problem.aspx?num=1530
 *
 * Strategy:
 * Xor and addition works the same as long as there are no carries, so we scan the numbers from the
 * right to the left until we get two bits that are equal to one. What we need to do then is to
 * make b equal zero at this point (and set everything to the right to zero as well to get the
 * smallest possible number), and then as soon as we stop propagating the carry, insert a 1. If
 * the number would overflow, increase a by one and set b to zero.
 *
 * Performance:
 * O(n), runs the tests in 0.015s using 536KB memory.
 */


#include <string>
#include <iostream>

int n;
std::string a, b;

int inc(std::string& a) {
    // Increases a by one, returns 1 if it overflows
    int c = 1;
    for(int i = a.size()-1; i >= 0 && c; i--)
        a[i] = '0' + (a[i]-'0' + c) % 2, c = a[i] == '0';
    return c;
}

std::pair<std::string, std::string> solve() {
    auto z = std::string(n, '0');
    if(!inc(b)) {
        int c = 0, ci = -1; // Carry and last index of carry
        for(int i = n-1; i >= 0; i--)
            if((a[i]-'0') + (b[i]-'0') + c > 1)
                c = 1, ci = i;
            else
                c = 0;
        if(ci == -1) // If we don't carry anywhere, xor = addition, so return the original numbers
            return { a, b };
        if(ci > 0) { // We carried, set the appropriate bit to 1 and everything to the right to 0
            b[ci-1] = '1';
            for(int i = ci; i < n; i++)
                b[i] = '0';
            return { a, b };
        }
    }
    // b overflowed, or the carry bit of addition overflowed - increase a
    if(inc(a))
        return { z, z }; // If even a overflowed, return the lexicographically smallest pair (0, 0)
    return { a, z }; // If a didn't overflow, return it together with b = 0
}

int main() {
    std::cin >> n >> a >> b;
    auto p = solve();
    std::cout << p.first << std::endl << p.second;
}
