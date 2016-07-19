/* 1274. Fractional Arithmetic - http://acm.timus.ru/problem.aspx?num=1274
 *
 * Strategy:
 * Straightforward parsing and calculation.
 *
 * Performance:
 * Constant, runs in 0.001s using 276KB memory.
 */

#include <sstream>
#include <iostream>
#include <unordered_map>

class Fraction
{
    static std::unordered_map<char, Fraction (Fraction::*) (const Fraction&)> map;

    long long gcd(long long a, long long b)
    {
        while(b != 0)
            std::swap(a, b), b = b % a;
        return a;
    }

    long long sgn(long long x)
    {
        return x ? x/std::abs(x) : 0;
    }

    Fraction parse(std::string str)
    {
        std::stringstream ss(str);
        long long a, b;
        char slash;
        ss >> a;
        if(ss >> slash >> b)
            return Fraction(a, b);
        else
            return Fraction(a);
    }

public:
    long long num, den;

    Fraction(std::string str)
    {
        std::stringstream ss(str);
        std::string a, b;
        ss >> a;
        if(ss >> b)
            *this = parse(a) + Fraction(a[0] == '-' ? -1 : 1)*parse(b);
        else
            *this = parse(a);
    }

    Fraction(long long a = 0, long long b = 1) : num(a), den(b)
    { 
        auto g = gcd(std::abs(a), std::abs(b));
        if(a)
            num /= g, den /= g;
        else
            num = 0, den = 1;
        long long s = sgn(num)*sgn(den);
        num = s*std::abs(num);
        den = std::abs(den);
    }
    Fraction call(char c, Fraction& b) { return (*this.*map[c])(b); }
    Fraction operator+ (const Fraction& a) { return Fraction(num*a.den + den*a.num, a.den*den); }
    Fraction operator- (const Fraction& a) { return Fraction(num*a.den - den*a.num, a.den*den); }
    Fraction operator/ (const Fraction& a) { return Fraction(num*a.den, a.num*den); }
    Fraction operator* (const Fraction& a) { return Fraction(num*a.num, den*a.den); }
};

std::ostream& operator<<(std::ostream& os, const Fraction& f)
{
    if(f.den == 1)
        os << f.num;
    else if(f.num == 0)
        os << "0";
    else if(std::abs(f.num) < std::abs(f.den))
        os << f.num << '/' << f.den;
    else
        os << f.num/f.den << ' ' << std::abs(f.num)%std::abs(f.den) << '/' << f.den;
    return os;
}

std::unordered_map<char, Fraction (Fraction::*) (const Fraction&)> Fraction::map = 
       { { '+', &operator+ }, { '-', &operator-}, { '*', &operator* }, { '/', &operator/ } };

int main()
{
    std::string str;
    std::getline(std::cin, str);
    Fraction a(str);
    std::getline(std::cin, str);
    char op = str[0];
    std::getline(std::cin, str);
    Fraction b(str);
    std::cout << a.call(op, b);
}