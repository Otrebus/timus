/* 1782. Jack's New Word - http://acm.timus.ru/problem.aspx?num=1782
 *
 * Strategy:
 * Brute-force dfs; try performing the 'replace' operation whenever there is an even number of bits,
 * otherwise try either removing a suffix or prefix, in an alternate fashion.
 *
 * Performance:
 * Runs the tests in 0.015s using 420KB memory.
 */

#include <vector>
#include <string>
#include <iostream>

struct op { // Represents a phrase
    int op;
    char b;
};
std::vector<op> ans;

bool dfs(std::string& s, int i, int j, bool side);

bool shrink(std::string& s, int i, int j, bool side) {
    // Performs 'replace' operation on string s from index i to j
    ans.push_back({ 2, 0 });
    std::string s2;
    bool good = true;
    for(int a = i; a < j; a+=2) { // Check that it's doable
        good &= s[a] != s[a+1];
        s2 += s[a];
    }
    if(good && dfs(s2, 0, s2.size(), side)) // If so, shrink and recurse
        return true;
    ans.pop_back();
    return false;
}

bool pop(std::string& s, int i, int j, bool side) {
    // Tries removing a prefix or a suffix (depending on side)
    if(side) {
        ans.push_back({ 0, s[i] });
        if(j == i+1 || dfs(s, i+1, j, !side))
            return true;
        ans.pop_back();
    } else {
        ans.push_back({ 1, s[j-1] });
        if(j == i+1 || dfs(s, i, j-1, !side))
            return true;
        ans.pop_back();
    }
    return false;
}

bool dfs(std::string& s, int i, int j, bool side) {
    // Tries replacing, prefixing and suffixing
    if((j-i)%2 == 0 && shrink(s, i, j, side))
        return true;
    return pop(s, i, j, side);
}

int main() {
    std::string s;
    std::cin >> s;

    dfs(s, 0, s.size(), true);

    // Output the ans array
    std::cout << ans.size() << std::endl;
    for(int i = ans.size()-1; i >= 0; i--) {
        auto op = ans[i].op;
        if(op < 2)
            std::cout << (!op ? "front" : "back") << " " << ans[i].b << std::endl;
        else
            std::cout << "double" << std::endl;
    }
}
