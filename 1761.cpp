/* 1761. Binary Palindrome - http://acm.timus.ru/problem.aspx?num=1761
 *
 * Strategy:
 * If we write some program to find the longest palindromes up to some size by brute-force, we can
 * beyond a certain size see that the largest palindrome is always formed by a sequence of numbers
 * similar to 10000101, 10000110, 100001(11) where we omit the digits in parenthesis from the
 * palindrome. The program below reproduces this string and outputs the length of it.
 * 
 * Performance:
 * O(log n), runs the tests in 0.031s using 9,936KB memory.
 */

#include <bitset>
#include <string>
#include <iostream>

bool ispalin(std::string s) { // Checks if the given string is a palindrome
    for(int i = 0; i < s.size()/2; i++)
        if(s[i] != s[s.size()-1-i])
            return false;
    return true;
}

std::string longest(std::string s) { // Finds the longest palindrome in a string
    for(int i = s.size(); i >= 1; i--)
        for(int j = 0; j + i <= s.size(); j++)
            if(ispalin(s.substr(j, i)))
                return s.substr(j, i);
    return "";
}

std::string brute(int n) { // Finds the longest binary palindrome up to number n
    std::string s;
    for(int i = 1; i <= n; i++) {
        std::string binary = std::bitset<16>(i).to_string();
        binary.erase(0, binary.find_first_not_of('0'));
        s += binary;
    }
    return longest(s);
}

std::string solve(std::string s) {

        if(s.length() < 6) // For small binary numbers we brute-force the solution
            return brute(std::stoi(s, nullptr, 2));

        // For larger sequences we find the sequence given in the header comment
        bool had1 = false;
        // First we check if the given number is >= 10[...]0111, otherwise we output
        // the palindrome which is one digit shorter
        for(int i = 1; i < s.size() - 3; i++)
            had1 |= (s[i] == '1');
        had1 |= s.substr(s.size()-3, 3) == "111";
        int sz = s.size() - !had1;

        std::string fst = "1" + std::string(sz-4, '0') + "1011";
        std::string out = fst + std::string(sz-4, '0');
        std::reverse(fst.begin(), fst.end());
        out += fst;
        return out;
}

int main() {
    std::string s;
    std::cin >> s;
    std::cout << solve(s).size();
}
