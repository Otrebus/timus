/* 1587. Flying Pig - http://acm.timus.ru/problem.aspx?num=1587
 *
 * Strategy:
 * For each non-zero span of numbers, if there is an even number of negative digits, compute the
 * product of these numbers and compare against the maximum. If there is an odd number of digits
 * compute the product of the span prefix before the last negative number, and the span suffix
 * after the first negative number and compare these against the maximum. A big integer class
 * is used and to minimize the amount of multiplications, we pre-multiply every product in chunks
 * up to the base of the big integer.
 *
 * Performance:
 * O(n^2), runs the tests in 0.312s using 824KB memory.
 */

#include <stdio.h>
#include <algorithm>
#include <vector>
#include <sstream>
#include <string>

const int base = 10000;

class BigInt
{
    std::vector<int> v;
public:
    BigInt(int p) : v(1, p) {}
    BigInt& operator*= (int p)
    {
        int carry = 0;
        for(int i = 0; i < v.size(); i++)
        {
            v[i] = v[i]*p + carry;
            carry = v[i]/base;
            v[i] %= base;
        }
        if(carry)
            v.push_back(carry);
        return *this;
    }
    bool operator< (const BigInt& b) const
    {
        if(b.v.size() != v.size())
            return v.size() < b.v.size();
        for(int i = v.size() - 1; i >= 0; i--)
            if(v[i] != b.v[i])
                return v[i] < b.v[i];
        return false;
    }
    std::string toString()
    {
        std::stringstream ss;
        int l = 0; // Number of digits, used for filling
        for(int b = base/10; b; b/=10)
            l++;
        ss << std::to_string(v[v.size()-1]); // First digit is not filled
        for(int i = v.size()-2; i >= 0; i--)
        {
            ss.width(l), ss.fill('0'); // Zero-fill the rest
            ss << std::to_string(v[i]);
        }
        return ss.str();
    }
};


void update(int a, int b, std::vector<int>& v, BigInt& ans)
{
    if(a == b)
        return;
    BigInt ret = 1;
    int h[4] = { 0, 0, 0, 0 }; // Histogram of digits
    for(int i = a; i < b; i++)
        h[std::abs(v[i])]++;
    for(int b = 1; b <= 3; b++)
    {
        while(h[b]) // While we have remaining digits of this histogram
        {           // (could be optimized a bit by carrying over to the next digit)
            // Precompute the product up to the base
            int s = 1;
            for(;b*s < base && h[b]; h[b]--)
                s *= b;
            ret *= s;
        }
    }
    ans = std::max(ans, ret);
}

int main()
{
    int n, x, max = -3;
    scanf("%d", &n);
    std::vector<int> v;
    for(int i = 0; i < n; i++)
    {
        scanf("%d", &x);
        max = std::max(max, x);
        v.push_back(x);
    }
    int z = 0, z2 = 0;
    BigInt ans(max);
    v.push_back(0); // Sentinel
    while(z2 < v.size())
    {
        int n1 = -1, n2 = -1, nn = 0;
        // Calculate the position of the first and last negative digit within this
        // nonzero span
        while(v[z2] != 0)
        {
            if(v[z2] < 0)
            {
                nn++;
                if(n1 == -1)
                    n1 = z2;
                n2 = z2;
            }
            z2++;
        }
        if(nn % 2 == 0) // Multiply the entire span
            update(z, z2, v, ans);
        else if(nn % 2 == 1) // Multiply before the last and after the first negative digits
            update(n1+1, z2, v, ans), update(z, n2, v, ans);
        z = ++z2;
    }
    printf(ans.toString().c_str());
}