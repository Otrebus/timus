/* 1074. Very Short Problem - http://acm.timus.ru/problem.aspx?num=1074
 *
 * Strategy:
 * Relatively straightforward parsing and outputting. There are a few edge cases to be mindful about
 * which are commented below.
 *
 * Performance:
 * Runs the tests in 0.015s using 228KB memory.
 */

#include <deque>
#include <string>
#include <cctype>
#include <iostream>
#include <sstream>

std::string s; // The string we are parsing
int p = 0; // The position in the string we are reading
bool fail; // Whether the parsing failed at some point

struct bigint { // Represents an integer, but hardly an actual bigint class
    std::deque<int> v;
    bool neg = false;

    bigint(std::deque<int> w = {}) : v(w) {}

    int toint() { // Turns the number into an int, capped at 100k
        int r = 0, i = 0;
        while(i < v.size() && !v[i])
            i++;
        for(; i < v.size() && r < 100000; i++)
            r = r*10 + v[i];
        return neg ? -r : r;
    }

    std::string str(bool b, int N = 1000) {
        // Turns the number into a string, with b denoting whether it's the integer or decimal part
        int i = 0;
        std::string ret;
        while(b && i < v.size() && v[i] == 0) // Trim leading zeroes of integer part
            i++;
        for(; i < (!b ? N : v.size()); i++) // Stop after N digits if decimal part
            ret += i < v.size() ? v[i]+'0' : '0';
        return ret.empty() ? "0" : ret;
    }

    bool isZero(int N=1000) {
        // Returns true if the number is all zeroes
        auto z = true;
        for(int i = 0; i < std::min((int)v.size(), N); i++)
            z &= !v[i];
        return z;
    }
};

struct fp { // Represents a floating point number
    bool neg = false;
    bigint M, m, e; // Integer part, decimal part, exponent

    void pow(int N) { // "Moves the dot" by pushing digits between the integer and decimal part
        int e = this->e.toint();
        if(e > 0) { // Power > 0
            // Because of the output size guarantee, the exponent must be bounded except if we are
            // all zeroes
            if(M.isZero() && m.isZero())
                return;
            while(e--) {
                auto a = m.v.size() ? m.v.front() : 0;
                if(m.v.size())
                    m.v.pop_front();
                M.v.push_back(a);
            }
        } else if(e) {
            // When e < 0, N limits the size of the shuffling we must do
            int r = std::max(e, (int)M.v.size() + N);
            while(e++ && r-- >= 0) {
                auto a = M.v.size() ? M.v.back() : 0;
                if(M.v.size())
                    M.v.pop_back();
                m.v.push_front(a);
            }
        }
    }
};

bool accept(char c) { // Optionally parses the given char
    return s[p] == c ? (p++, true) : false;
}

bool parseSign() { // Optionally parses a sign, returns true if negative
    if(accept('-'))
        return true;
    accept('+');
    return false;
}

bigint parseInt() { // Parses an integer
    std::deque<int> v;
    if(!std::isdigit(s[p]))
        fail = true;
    while(std::isdigit(s[p]))
        v.push_back(s[p++]-'0');
    return v;
}

bigint acceptInt() { // Optionally parses an integer
    if(std::isdigit(s[p]))
        return parseInt();
    return bigint();
}

fp parse() { // Parses a floating point according to the rules given
    fp n;
    n.neg = parseSign();
    if(accept('.')) {
        n.m = parseInt();
    } else {
        n.M = parseInt();
        if(accept('.'))
            n.m = parseInt();
    }
    if(accept('e') || accept('E')) {
        auto neg = parseSign();
        n.e = parseInt();
        n.e.neg = neg;
    }
    if(p != s.size()) // Stuff exists after a correct floating point number
        fail = true;
    return n;
}

std::string print(fp p, int N) { // Prints a floating point number to the given accuracy
    std::stringstream ss;
    auto M = p.M,  m = p.m;

    p.pow(N);

    if(p.neg && (!p.m.isZero(N) || !p.M.isZero()))
        ss << '-';
    ss << p.M.str(true);
    if(N)
        ss << "." << p.m.str(false, N);
    return ss.str();
}

int main() {
    while(true) {
        std::string L;
        std::getline(std::cin, s);
        if(s == "#")
            break;
        std::getline(std::cin, L);
        p = 0;
        fail = false;
        int N = std::stoi(L);
        auto s = print(parse(), N);
        if(fail)
            std::cout << "Not a floating point number" << std::endl;
        else
            std::cout << s << std::endl;
    }
}
