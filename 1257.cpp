/* 1257. Hyphenation - http://acm.timus.ru/problem.aspx?num=1257
 *
 * Strategy:
 * Greedy hyphenation, picking the option that gives the least carry.
 *
 * Performance:
 * O(MW^3) where M is the number of words in the text and W is the maximum size of a word. Runs the
 * tests in 0.046s using 660KB memory.
 */

#include <vector>
#include <string>
#include <unordered_map>
#include <stdio.h>
#include <cctype>
#include <algorithm>

std::unordered_map<std::string, std::vector<int>> H; // Maps words to hyphen positions

const int W = 40;

std::string lower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), [](char c){ return std::tolower(c); });
    return s;
}

std::string hyphenate(std::string s, int& w) {
    // Hyphenates the given word s starting at position w in the line, giving a new w
    std::string mint;
    int maxj = -1, maxi = -1, minc = 100;

    for(int i = 0; i < s.size(); i++) { // For every suffix of the word
        auto t = s.substr(i);
        auto Wl = w - (int)s.size(); // Line position at the suffix start
        if(H.find(lower(t)) != H.end()) // If the suffix is hyphenable
            for(auto j : H[lower(t)]) // For every hyphen position
                if(Wl+i+j+1 <= W && t.size()-j < minc) // Check if it fits, update best carry
                    minc = t.size()-j, maxj = j, maxi = i, mint = t;
    }

    if(maxi != -1) { // If we found some hyphenable construct, add it with linebreak
        s = s.substr(0, maxi) + mint.substr(0, maxj) + "-\n" + mint.substr(maxj);
        w = mint.substr(maxj).size();
    }
    else { // Otherwise, output the whole word
        w = s.size();
        s = "\n" + s;
    }
    return s;
}

int main() {
    char c, h[45];
    int N, p;
    std::string s, t, out;
    scanf("%d", &N);
    while(N--) {
        scanf("%s", h);
        std::string s(h);
        for(int i = 0; i < s.size(); i++)
            if(s[i] == '-')
                p = i;
        H[lower(s).replace(p, 1, "")].push_back(p);
    }
    for(getc(stdin); (c = getc(stdin)) != EOF; s += c);

    std::string t, out; // Current word, output
    for(int i = 0, w = 0; i < s.size();) {
        if(w >= W) // Line width exceeded, newline
            out += '\n', w = 0;
        if(!isalpha(s[i]) && s[i] != '\n') // Anything nonalphabetical is read verbatim
            out += s[i++], w++;
        else {
            while(i < s.size() && isalpha(s[i]))
                t += s[i++], w++; // While alphabetics are read, build word
            if(t.size()) { // Check if the word fits the line, otherwise hyphenate
                out += w <= W ? t : hyphenate(t, w);
                t = "";
            }
        }
        if(i < s.size() && s[i] == '\n')
            out += s[i++], w = 0;
    }
    printf("%s", out.c_str());
}
