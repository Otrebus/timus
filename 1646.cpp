/* 1646. Godzilla Strikes Back! - http://acm.timus.ru/problem.aspx?num=1646
 *
 * Strategy:
 * When splicing together two DNA sequences, the newly created instances of Godzilla's DNA will
 * exclusively occur over the junction of those two DNA sequences. Hence, for any DNA sequence,
 * we only need to store the number of instances of Godzilla's DNA inside that sequence, together
 * with the prefix and suffix of that sequence up to the length of Godzilla's DNA. When we add
 * two sequences together, we then update this info appropriately.
 *
 * Performance:
 * O(mn^2) where n is the length of Godzilla's DNA, runs the tests in 0.015s using 360KB memory.
 */

#include <algorithm>
#include <iostream>
#include <string>

std::string dna; // Godzilla's DNA
const int mod = 1000000007;

struct segment // Dna sequence
{
    std::string pre, suf; // Prefix and suffix of this sequence
    unsigned int n; // Number of instances of Godzilla's DNA inside this sequence
    int length;
} segments[103];

// Returns the number of instances of key inside str
int find(const std::string& str, const std::string& key)
{
    int ret = 0;
    for(int i = 0; i <= (int) str.size() - (int) key.size(); i++)
        if(str.substr(i, key.size()) == key)
            ret++;
    return ret;
}

// Initializes a DNA sequence from an istream
std::istream& operator>>(std::istream& is, segment& s)
{
    std::string str;
    is >> str;
    // Initialize the prefix
    s.pre = str.substr(0, dna.size()-1);
    s.length = str.length();
    // Initialize the suffix
    for(int i = str.size() - std::min(dna.size() - 1, str.size()); i < str.size(); i++)
        s.suf.push_back(str[i]);
    s.n += find(str, dna);
    return is;
}

// Splices two DNA sequences together and updates their info
segment operator+(segment& lhs, segment& rhs)
{
    segment s;
    s.length = lhs.length + rhs.length;
    std::string str = lhs.suf + rhs.pre;
    int l = std::max(0, 1 + (int) lhs.suf.size() - (int) dna.size());
    int size = lhs.suf.size() + dna.size() - 1 - l;
    s.n = (lhs.n + rhs.n + find(str.substr(l, size), dna)) % mod;
    // The prefix of the new sequence might span through lhs into rhs
    s.pre = lhs.pre + (lhs.pre.size() == lhs.length ? rhs.pre : "");
    s.pre = s.pre.substr(0, dna.size() - 1);
    // Same as above but for the suffix
    s.suf = (rhs.pre.size() == rhs.length ? lhs.suf : "") + rhs.suf;
    s.suf = s.suf.substr(std::max(1 + (int) s.suf.size() - (int) dna.size(), 0), dna.size() - 1);
    return s;
}

int main()
{
    int m, x, y;
    std::cin >> dna >> segments[1] >> segments[2] >> m;
    for(int i = 3; i <= m+2; i++)
    {
        std::cin >> x >> y;
        segments[i] = segments[x] + segments[y];
    }
    std::cout << segments[m+2].n;
}