/* 1462. Uncle Scrooge's Gold - http://acm.timus.ru/problem.aspx?num=1462
 *
 * Strategy:
 * To produce all strings of length n, we can either get any string of length n-2 and add '01' as
 * a prefix, and any string of length n-1 and add '1', which gives the number of strings of length
 * n equal to F[n] = F[n-1] + F[n-2] which is the Fibonacci function. Since the number of strings
 * of length 1 and 2 is F[3] and F[4], the number of strings of length N in general is F[n+2] and
 * the problem asks us to calculate F[2*n]/F[n] which equals F[n+1] + F[n-1], which we calculate.
 *
 * Performance:
 * O(N^2), runs the tests in 0.531s using 7,592KB memory.
 */

#include <vector>
#include <string>
#include <stdio.h>

struct BigInt {
    using ll = unsigned long long;

    std::vector<ll> num;
    ll base = 1e18;
    ll basewidth = 18;

    BigInt(ll i) {
        num.push_back(i);
    }

    BigInt operator+ (const BigInt& b2) {
        BigInt result(0);
        auto& rnum = result.num;
        auto num2 = b2.num;

        int size = std::max(num.size(), num2.size());
        rnum.resize(size), num2.resize(size), num.resize(size);

        ll carry = 0;
        for(int i = 0; i < size; i++) {
            ll res = (num[i] + num2[i] + carry);
            rnum[i] = res%base;
            carry = res/base;
        }
        if(carry)
            rnum.push_back(carry);

        return result;
    }

    std::string str() {
        std::string ret;
        for(int i = num.size()-1; i >= 0; i--) {
            std::string s = std::to_string(num[i]);
            ret += (i < num.size()-1 ? std::string(basewidth-s.size(), '0') : "") + s;
        }
        return ret;
    }
};

int main() {
    BigInt* B[3] = { new BigInt(0), new BigInt(1), new BigInt(1) };
    int n;
    scanf("%d", &n);
    for(int i = 0; i < n-1; i++) {
        BigInt* b = new BigInt(0);
        *b = *B[1] + *B[2];
        delete B[0];
        B[0] = B[1];
        B[1] = B[2];
        B[2] = b;
    }
    printf("%s", (*B[0] + *B[2]).str().c_str());
}
