/* 1531. Zones on a Plane - http://acm.timus.ru/problem.aspx?num=1531
 *
 * Strategy:
 * If we draw a few iterations, we see that the figure gets "teeth" on the diagonal sides and the
 * rectilinear sides (alternately) that then get filled in, and then roughly double compared to the
 * previous teething, and so on, leading to the simple recurrence given in the main function.
 *
 * Performance:
 * O(N^2), runs the tests in 0.015s using 300KB memory.
 */

#include <iostream>
#include <vector>
#include <sstream>

struct BigInt {
    std::vector<int> v;
    static const int base = 10000;

    BigInt(int a = 0) {
        if(a)
            v.push_back(a);
    }

    BigInt operator* (int p) {
        int carry = 0;
        for(int i = 0; i < v.size(); i++) {
            v[i] = v[i]*p + carry;
            carry = v[i]/base;
            v[i] %= base;
        }
        if(carry)
            v.push_back(carry);
        return *this;
    }
    BigInt operator*= (int p) { return *this = *this*p; }

    BigInt operator+(BigInt b) {
        BigInt c;
        int carry = 0;
        v.resize(std::max(b.v.size(), v.size()), 0);
        b.v.resize(std::max(b.v.size(), v.size()), 0);
        for(int i = 0; i < v.size(); i++) {
            c.v.push_back((v[i]+b.v[i]+carry) % base);
            carry = (v[i]+b.v[i]+carry)/base;
        }
        if(carry)
            c.v.push_back(carry);
        return c;
    }
    BigInt operator+= (int p) { return *this = *this + p; }
};


std::ostream& operator<< (std::ostream& os, const BigInt& b) { // Prints out a BigInt
    int l = 0;
    for(int b = BigInt::base/10; b; b/=10)
        l++;
    os << std::to_string(b.v[b.v.size()-1]);
    for(int i = b.v.size()-2; i >= 0; i--) {
        os.width(l), os.fill('0');
        os << std::to_string(b.v[i]);
    }
    return os;
}

int main() {
    int n;
    std::cin >> n;

    int lo[] = { 0, 1, 4, 8, 12, 20 }; // Our recurrence doesn't work for n < 6
    if(n < 6)
        return std::cout << lo[n], 0;

    BigInt R = 1, D = 2; // Number of rectilinear and diagonal teeth (of 1/4th of the figure)
    for(int i = 6; i <= n; i++)
        if(i % 2 == 0)
            R *= 2, D += 1; // Rectilinear teeth double, diagonal teeth get filled in
        else
            R += 1, D *= 2; // Diagonal teeth double, rectilinear teeth get filled in

    std::cout << (R + D + 2)*4; // Plus two extra teeth on each edge that always rode along
}
