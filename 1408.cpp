/* 1408. Polynomial Multiplication - http://acm.timus.ru/problem.aspx?num=1408
 *
 * Strategy:
 * Straightforward parsing and updating.
 *
 * Performance:
 * O(n^2*log n), where n is the size of the input, runs the tests in 0.015s using 332KB memory.
 */

#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <numeric>

const char nc = 1+'z'-'a';

struct term { // Represents one term of type c*x^a_x*y^a_y*...
    int c; // The constant factor
    int a[nc]; // The power of each variable

    term() : c(0) { std::memset(a, 0, sizeof(a)); }
    bool operator <(const term& b) const {
        // Sorts the terms by the order given in the problem statement
        auto x = std::accumulate(a, a+nc, 0), y = std::accumulate(b.a, b.a+nc, 0);
        return std::pair(x, std::vector(a, a+nc)) > std::pair(y, std::vector(b.a, b.a+nc));
    }
    term operator *(const term& t) const {
        term s = *this;
        for(int i = 0; i < nc; i++)
            s.a[i] += t.a[i];
        s.c *= t.c;
        return s;
    }
};

int p = 0;
std::string str;
std::set<term> s;

char peek() { return p < str.size() ? str[p] : 0; } // Peeks the next character in the input
char next() { return str[p++]; } // Returns the next character in the input and advances
bool accept(char c) { // Optionally reads the next character c from the stream
    return (p < str.size() && str[p] == c) ? ++ p : false;
}

void addToSet(term t) { // Inserts a term into the set of terms
    auto it = s.find(t);
    if(it != s.end()) { // If the term exists, add the constant
        auto u = *it;
        s.erase(t);
        t.c += u.c;
    } 
    s.insert(t);
}

int parseInt() { // Parses an integer
    int n = 0;
    while(std::isdigit(peek()))
        n = n*10 + next() - '0';
    return n;
}

void parseFactor(term& t) { // Parses a factor in a product
    char c = next();
    t.a[c-'a'] += (accept('^') ? parseInt() : 1);
}

void parseProduct(term& t) { // Parses a product
    parseFactor(t);
    if(accept('*'))
        parseProduct(t);
}

void parseTerm() { // Parses a single term
    term t;
    bool neg = accept('-');
    accept('+');
    int n = 1;
    if(std::isdigit(peek())) { // Has a coefficient
        n = parseInt();
        if(accept('*'))
            parseProduct(t);
    } else
        parseProduct(t);
    t.c = (neg ? -1 : 1)*n;
    addToSet(t);
}

void parseTerms() { // Parses a set of terms
    parseTerm();
    if(peek())
        parseTerms();
}

void readExpression() { // Parses an expression
    std::getline(std::cin, str);
    str.erase(remove_if(str.begin(), str.end(), isspace), str.end());
    parseTerms();
}

void printExpression() { // Prints an expression according to the rules given in the statemnet
    if(s.empty())
        std::cout << "0";
    for(auto it = s.begin(); it != s.end(); it++) {
        if(!it->c) // Don't print terms with a coefficient of zero
            continue;

        int n = 0; // Number of nonzero powers
        for(auto c : it->a)
            n += bool(c);

        if(it->c > 0 && it != s.begin()) // Positive terms after the first one are prefixed by +
            std::cout << " + ";
        if(it->c < 0) // Negative terms have a minus, if it's the first one, the minus has no space
            std::cout << (it == s.begin() ? "-" : " - ");
        if(std::abs(it->c) != 1 || !n) // Print the coefficient and an asterisk
            std::cout << std::abs(it->c) << (n ? "*" : "");

        for(int i = 0; i < nc; i++) { // Print all the powers
            if(it->a[i]) {
                n--;
                std::cout << char('a' + i);
                if(it->a[i] > 1)
                    std::cout << "^" << (int)it->a[i];
                std::cout << (n ? "*" : "");
            }
        }
    }
}

int main() {
    readExpression();
    std::set<term> s1 = s;
    p = 0, s.clear();
    readExpression();
    std::set<term> s2 = s;
    s.clear();

    for(auto& t1 : s1) // Cross-multiply the terms, and add to the set if the constant is nonzero
        for(auto& t2 : s2)
            if((t1*t2).c)
                addToSet(t1*t2);

    printExpression();
}
