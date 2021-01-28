/* 1165. Subnumber - http://acm.timus.ru/problem.aspx?num=1165
 *
 * Strategy:
 * Try to categorise the given string into these three types:
 * 
 *   1) (x-1][x][x+1][x+2][x+3)
 *   2) (x-1][x)
 *   3)      (x)
 * 
 * Where [x] is a full number and [x) is the prefix of a number, etc. Whenever a candidate
 * sequence is found, calculate the position of x in S recursively. For example, the position
 * of 123 is 24*3 + (the position of 99), since with have 24 3-digit numbers and the numbers
 * 1-99 before them. Then, adjust for the position of x in the sequence. The answer is the
 * minimum of all candidate positions.
 * 
 * Performance:
 * Cubic in the size of the input, runs the tests in 0.328s using 1,480KB memory.
 */

#include <vector>
#include <string>
#include <iostream>

struct BigInt { // Standard-ish BigInt class, no comments necessary

    std::vector<int> num;

    BigInt(const std::string& str) {

        for(int i = str.size() - 1; i >= 0; i--)
            num.push_back(str[i]-'0');

        clean();
    }

    void clean() {
        while(num.size() && !num.back())
            num.pop_back();
    }

    BigInt operator++ () {
        int carry = 1;
        for(int i = 0; i < num.size(); i++) {
            int res = num[i] + carry;
            num[i] = res%10;
            carry = res/10;
        }
        if(carry)
            num.push_back(carry);
        return *this;
    }

    bool operator> (const BigInt& b) const {
        std::string x = str(), y = b.str();
        if(x.size() != y.size())
            return x.size() > y.size();
        return x > y;
    }

    BigInt operator-- () {
        int borrow = -1;
        for(int i = 0; i < num.size(); i++) {
            int res = num[i] + borrow;
            num[i] = (10+res)%10;
            borrow = -(res < 0);
        }
        
        clean();
        return *this;
    }

    BigInt operator+ (const BigInt& b2) {
        BigInt result("0");
        auto& rnum = result.num;
        auto num2 = b2.num;

        int size = std::max(num.size(), num2.size());
        rnum.resize(size), num2.resize(size), num.resize(size);

        int carry = 0;
        for(int i = 0; i < size; i++) {
            int res = (num[i] + num2[i] + carry);
            rnum[i] = res%10;
            carry = res/10;
        }
        if(carry)
            rnum.push_back(carry);

        return result;
    }

    BigInt operator* (int n) {
        BigInt result("0");
        auto& rnum = result.num;
        rnum.resize(num.size());
        
        int carry = 0;
        for(int i = 0; i < num.size(); i++) {
            int res = (num[i]*n + carry);
            rnum[i] = res%10;
            carry = res/10;
        }

        if(carry)
            rnum.push_back(carry);
        return result;
    }

    BigInt operator- (int n) {
        BigInt result("0");
        auto& rnum = result.num;
        rnum.resize(num.size());
        int borrow = -n;

        for(int i = 0; i < num.size(); i++) {
            int res = num[i] + borrow;
            rnum[i] = (10000+res)%10;
            borrow = (res - 9)/10;
        }
        
        result.clean();
        return result;
    }

    size_t size() const {
        return num.size();
    }

    explicit operator bool() const {
        return num.size() && !(num.size() == 1 && !num[0]);
    }

    std::string str() const {
        std::string s;
        for(int i = num.size()-1; i >= 0; i--)
            s += num[i] + '0';
        return !s.size() ? "0" : s;
    }
};

BigInt pos_(const BigInt& inp) {
    if(!inp)
        return BigInt("0");
    BigInt b = inp.str();
    BigInt result = BigInt("0");

    b.num.back()--;
    result = result + (++b)*inp.size();
    
    return result + pos_(BigInt(std::string(inp.size()-1, '9')));
}

BigInt pos(BigInt inp) {
    // Calculates the position of the given number in S
    return ++pos_(--inp);
}

int main() {
    std::string s;
    std::cin.tie(nullptr);
    std::ios::sync_with_stdio(false);
    std::cin >> s;
    
    BigInt m(std::string(1000, '9'));
    auto setMin = [&] (const BigInt& c) { m = c > m ? m : c; };

    // Sequence type 1 as explained in the header comments
    for(int i = 1; i < s.size(); i++) {

        if(s[i] == '0')
            continue;

        for(int j = i; j < s.size(); j++) {
            BigInt b(s.substr(i, j));
            BigInt b1 = b, b2 = b;

            --b2;
            // Reverse once
            if(!b2 || b2.size() < i || s.substr(0, i) != b2.str().substr(b2.size()-i, i))
                continue;

            bool bad = false;
            // Increase our number forward until we get a mismatch
            for(int k = i + b1.size(); k < s.size(); k += b1.size()) {
                ++b1;
                if(b1.str().substr(0, s.size()-k) != s.substr(k, b1.size())) {
                    bad = true;
                    break;
                }
            }
            // If no mismatch this is a full sequence
            if(!bad) {
                setMin(pos(b) - i);
                break;
            }
        }
    }

    // Sequence type 2
    for(int i = 1; i < s.size(); i++) {
        if(s[i] == '0')
            continue;

        // Here we try to make the incremented prefix (up to index i) of the sequence, and
        // the suffix (after i) overlap maximally, so for the sequence 0370 we can form
        // 04 from 03, and attach it to 70 overlappingly to form 704
        auto l = s.substr(0, i);
        BigInt b(l);
        ++b;
        auto a = b.str();
        int d = std::min(l.size(), b.size());
        l = l.replace(std::max(0, (int)l.size()-(int)a.size()), d, a.substr(a.size() - d, d));

        auto sz = s.size();
        // j is the size of the overlap, we start with the biggest one possible which would
        // give the smallest resulting number, so we can break once we find one
        for(int j = std::min((int)sz - i, i); j >= 0; j--) {

            if(s[0] == '0' && j == sz - i)
                continue;

            bool bad = false;
            // Check if the overlap works
            for(int k = 0; k < j; k++) {
                if(s[sz-j+k] != l[k]) {
                    bad = true;
                    break;
                }
            }
            if(!bad) {
                BigInt x = BigInt(s.substr(i, sz-j) + l.substr(j));
                int len = x.num.size();
                setMin(pos(x) - i);
                break;
            }
        }
    }

    // Sequence type 3, just attach a 1 in front of it, or if there's no zero use the sequence as is
    setMin(++pos('1' + s));
    if(s[0] != '0')
        setMin(pos(s));

    std::cout << m.str();
}
