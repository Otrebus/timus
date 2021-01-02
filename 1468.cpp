/* 1468. Fraction - http://acm.timus.ru/problem.aspx?num=1468
 *
 * Strategy:
 * Run standard long division, map the remainder of each iteration to the iteration number and break
 * whenever we've seen that remainder before (this being the only 'state' of the algorithm, we would
 * loop).
 *
 * Performance:
 * Runs the test cases in 0.25s using 736KB memory.
 */

#include <iostream>
#include <vector>
#include <unordered_map>

std::vector<char> d;

void solve(int a, int b, int k) {
    std::vector<int> out; // The result
    std::unordered_map<int, int> s; // The map between remainder to iteration

    int r, i;
    for(r = a, i = 0; ; r *= k) { // Long division
        out.push_back(r/b);
        if(r/b)
            r = r%b;
        if(!r || s[r]) // We've seen this remainder before, or it being 0 we finished
            break;
        s[r] = ++i;
    }

    // The first "digit" in the above algorithm can be larger than the base, so handle manually
    std::vector<int> v;
    int w = out[0];
    do {
        v.push_back(w % k);
        w /= k;
    } while(w);

    for(auto it = v.rbegin(); it < v.rend(); it++)
        std::cout << d[*it];
    if(out.size() > 1)
        std::cout << ".";

    for(int i = 1; i < out.size(); i++) {
        if(s[r] == i) // Being the index of the first repeated remainder, digits repeat from here on
            std::cout << "(";
        std::cout << d[out[i]];
    }
    if(s[r])
        std::cout << ")";
    std::cout << std::endl;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    for(char i = '0'; i <= '9'; i++)
        d.push_back(i);
    for(char c = 'A'; c <= 'Z'; c++)
        d.push_back(c);

    while(true) {
        int a, b, k;
        std::cin >> a >> b;
        if(!(a*b))
            return 0;
        std::cin >> k;
        solve(a, b, k);
    }
}
