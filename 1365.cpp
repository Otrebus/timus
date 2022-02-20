/* 1365. Testing Calculator - http://acm.timus.ru/problem.aspx?num=1365
 *
 * Strategy:
 * Straightforward recursive-descent parsing where we keep track of a single state - whether an
 * encountered semicolon belongs to this grammatical rule or the parent rule. (With a better
 * formulation of the grammar that state might be avoidable)
 *
 * Performance:
 * Linear in the length of the input, runs in 0.015s using 224KB memory.
 */

#include <iostream>
#include <string>
#include <cctype>
#include <iostream>
#include <vector>

std::string s;
int k, n;

int parseConc(int);

bool accept(char c) { // Returns true and advances the current pointer if it points to character c
    return s[k] == c ? ++k : 0;
}

int parseDiv(int a = 0, int d = 0) { // Parses division (the parameters are for code golfing)
    for(a = parseConc(1), d = 0; accept(';'); d = parseConc(1), a = (!d ? 0 : a/d));
    return a;
}

int parseMul(int a = 0) { // Parses a multiplication operation
    for(a = parseConc(1); accept(';'); a *= parseConc(1));
    return a;
}

int parseSum(int a = 0) { // Parses a summation operation
    for(a = parseConc(1); accept(';'); a += parseConc(1));
    return a;
}

int parseDigits(int S = 0) { // Parses a number
    for(S = 0; isdigit(s[k]); S = S*10 + s[k++] - '0');
    return S;
}

int parseExp(int d) { // Parses an expression (d is whether we are greedily eating semicolons)
    if(isdigit(s[k]))
        return parseDigits();
    if(accept('+'))
        return parseSum();
    if(accept('/'))
        return parseDiv();
    if(accept('*'))
        return parseMul();
    if(accept('(')) {
        int x = parseConc(0);
        accept(')');
        return x;
    }
    return -1;
}

int parseConc(int d) { // Parses a concatenation (a set of expressions next to each other)
    std::vector<int> v;
    for(int x = parseExp(d); x != -1 || v.empty(); x = parseExp(d)) {
        v.push_back(x);
        if(s[k] == ';') {
            if(d) // This semicolon belongs to the parent expression
                break;
            k++;
            v.push_back(parseConc(d));
        }
    };
    std::string out;
    for(auto x : v)
        out += std::to_string(std::abs(x));
    return out == "" ? 1 : std::stoi(out);
}

int main() {
    for(int i = 1; std::cin >> s; i++, k = 0)
        std::cout << "Expression " << i << " evaluates to: " << parseConc(0) << std::endl;
}
