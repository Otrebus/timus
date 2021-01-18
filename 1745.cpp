/* 1745. Yet Another Answer - http://acm.timus.ru/problem.aspx?num=1745
 *
 * Strategy:
 * Sort the words by partitioning them into overflowing words (sorted by max underflow of any
 * prefix) followed by underflowing (similarly). Here, 'overflowing' means a word has more opening
 * parentheses than closing parentheses and a word having an underflowing prefix means it's
 * 'dipping'.
 *   Then, build sequences (memoized by overflow) by appending each word in the sorted order to each
 * of the sequences we have built so far, starting with the empty sequence.
 *
 * Performance:
 * O(NM), where M is the total size of the output, runs the test cases in 0.218s using 17,344KB.
 */

#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <unordered_map>

struct word {
    int m; // The dip
    int i; // The index of the word pre-sorting
    int l; // The length of the word
    int t; // The overflow
} words[1001];

struct prev {
    int i, p; // Index of word, index of previous prev, used to build the winning word sequence
};

struct entry { // The entry into the DP table, indexed by opening parentheses surplus
    int p, l; // Index of prev, length of word
};

std::vector<prev> p;
std::unordered_map<int, entry> B, B2; // Our dp table, indexed by overflow

int main() {

    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    p.push_back( { 0, 0 } ); // Sentinel for prev
    B[0] = { 0, 0 }; // Empty sequence
    B2[0] = { 0, 0 };
    int n;
    std::cin >> n;

    for(int i = 1; i <= n; i++) {
        std::string s;
        std::cin >> s;

        int t = 0, m = 0;
        for(auto c : s)
            m = std::min(m, t += c == '(' ? 1 : -1);
        words[i] = { m, i, (int) s.size(), t };
    }

    std::sort(words+1, words+n+1, [] (const word& a, const word& b) { // Sort as described
        return a.t >= 0 && (b.t < 0 || a.m > b.m) || b.t < 0 && a.m - a.t < b.m - b.t;
    });

    auto b = &B;
    auto b2 = &B2;

    for(int i = 1; i <= n; i++) {
        for(auto it = b->begin(); it != b->end(); it++) { // For each sequence

            auto k = it->first;
            auto e = it->second;

            if(k + words[i].m < 0) // Can't add this word to the sequence, underflowing
                continue;

            int j = k + words[i].t;
            int l = e.l + words[i].l;

            if(l > (*b2)[j].l) { // Is the solution longer than the one in this entry?
                p.push_back( {i, e.p} ); // Save the word to the 'previous' tree
                (*b2)[j] = { (int) p.size()-1, l }; // Replace the entry
            }
        }
        *b = *b2;
    }

    std::vector<int> output;
    int l = 0;

    for(auto& v = p[(*b)[0].p]; v.i; v = p[v.p]) {
        output.push_back(v.i);
        l += words[v.i].l;
    }
    std::cout << l << " " << output.size() << "\n";

    for(auto s = output.rbegin(); s < output.rend(); s++) {
        std::cout << words[*s].i << " ";
    }
}
