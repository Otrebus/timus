/* 1265. Mirror - http://acm.timus.ru/problem.aspx?num=1265
 *
 * Strategy:
 * Reflect one of the points to the other side of the mirror and check if the line from the other
 * point to the reflected point intersects the mirror. Since the problem tests for insane levels of
 * accuracy, we multiply all coordinates by 1e6 to form integers, and use a BigInt class to check
 * the resulting inequality (as often, see below for more detailed comments).
 *
 * Performance:
 * O(1), runs the test cases in 0.015s using 300KB memory.
 */

#include <vector>
#include <string>
#include <iostream>
#include <cmath>

class BigInt
{
    static const int base = 10;
    static const int basewidth = 1;
public:
    std::vector<int> v;
    bool neg;
    BigInt(bool b) : neg(b) {}
    BigInt(int a = 0) : neg(false)
    {
        if(a < 0) {
            neg = true;
            a = std::abs(a);
        }
        while(a) {
            v.push_back(a%base);
            a/=base;
        }
    }

    void trim() {
        while(v.size() && v.back() == 0)
            v.pop_back();
        if(v.empty())
            neg = false;
    }

    BigInt abs() {
        BigInt res = *this;
		res.neg = false;
		return res;
    }

    BigInt operator-() const {
		BigInt res = *this;
		res.neg = !neg;
		return res;
	}

    BigInt operator+(BigInt b)
    {
        if(b.neg != neg)
            return *this - (-b);
        BigInt c(neg);
        int carry = 0;
        v.resize(std::max(b.v.size(), v.size()), 0);
        b.v.resize(std::max(b.v.size(), v.size()), 0);
        for(int i = 0; i < v.size(); i++)
        {
            c.v.push_back((v[i]+b.v[i]+carry) % base);
            carry = (v[i]+b.v[i]+carry)/base;
        }
        if(carry)
            c.v.push_back(carry);
        c.trim();
        return c;
    }

    BigInt operator-(BigInt b)
    {
        if(b.neg != neg)
            return *this + (-b);
        if(this->abs() < b.abs())
            return -(b - *this);
        BigInt c(neg);
        int carry = 0;
        v.resize(std::max(b.v.size(), v.size()), 0);
        b.v.resize(std::max(b.v.size(), v.size()), 0);
        for(int i = 0; i < v.size(); i++)
        {
            int newcarry = 0;
            if(v[i] < (b.v[i]+carry))
                newcarry = 1; 
            c.v.push_back(((base+v[i])-(b.v[i]+carry)) % base);
            carry = newcarry;
        }
        c.trim();
        return c;
    }

    BigInt operator*(BigInt b) {
        BigInt res;
        int n = v.size();
        for(int i = 0; i < n; i++) {
            BigInt c(neg);
            c.v = std::vector<int>(i, 0);
            int carry = 0;
            v.resize(std::max(b.v.size(), v.size()), 0);
            b.v.resize(std::max(b.v.size(), v.size()), 0);
            for(int j = 0; j < b.v.size(); j++) {
                int res = v[i]*b.v[j]+carry;
                carry = res/base; 
                c.v.push_back(res%base);
            }
            if(carry)
                c.v.push_back(carry);
            res = res + c;
        }
        res.neg = b.neg ^ neg;
        res.trim();
        return res;
    }

    bool operator<(BigInt b)
    {
        if(neg && b.neg)
            return (-b < -*this);
        if(neg ^ b.neg)
            return neg;
        while(v.size() && v.back() == 0)
            v.pop_back();
        while(b.v.size() && b.v.back() == 0)
            b.v.pop_back();
        if(v.size() != b.v.size())
            return v.size() < b.v.size();
        for(int i = v.size()-1; i >= 0; i--)
        {
            if(v[i] != b.v[i])
                return v[i] < b.v[i];
        }
        return false;
    }

    bool operator<=(BigInt b) {
        return !(b < *this);
    }
};

using ld = long double;
ld eps = 1e-15;

struct point {
    BigInt x, y;
    point(BigInt x, BigInt y) : x(x), y(y) {}
    point(ld a, ld b) : x(int(std::round(a*1000000))), y((int)std::round(b*1000000)) { }
    point operator-(point p) { return { x-p.x, y-p.y }; }
    point operator+(point p) { return { x+p.x, y+p.y }; }
    BigInt operator*(point p) { return x*p.x + y*p.y; }
    point operator*(BigInt f) { return { f*x, f*y }; }
    BigInt d2() { return x*x + y*y; }
    BigInt operator%(point p) { return x*p.y - p.x*y; }
};

int main() {

    ld p1x, p1y, p2x, p2y, m1x, m1y, m2x, m2y;
    std::cin >> p1x >> p1y >> p2x >> p2y >> m1x >> m1y >> m2x >> m2y;
    point p1(p1x, p1y), p2(p2x, p2y), m1(m1x, m1y), m2(m2x, m2y);

    if(((m2-m1)%(p1-m1))*((m2-m1)%(p2-m1)) < 0) // The points are on different sides of the mirror
        return std::cout << "INVISIBLE", 0;

    auto d = (m2-m1).d2();
    auto a = m2-m1, b = p2-m1;

    // Reflect one of the points, form the equation m1 + (m2-m1)*t = p1 + (r-p1)s where r is the
    // reflection point to check that 0 <= s <= 1, multiply both sides by d to remove any divisions,
    // solve by Cramer's rule and multiply by the determinant to get an all-integer inequality
    auto u = m1*d + a*2*(b*a)-(b+p1)*d, v = (m1-m2)*d, c = (m1-p1)*d;
    auto L = u.x*c.y-c.x*u.y, D = u.x*v.y-v.x*u.y; // Num/denom determinants of the Cramer equation
    // D could be negative which reverses the inequality
    auto B = BigInt(0) <= D ? (L <= D && BigInt(0) <= L) : (D <= L && L <= 0); 

    std::cout << (B ? "VISIBLE" : "INVISIBLE");
}
