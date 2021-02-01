/* 1269. Obscene Words Filter - http://acm.timus.ru/problem.aspx?num=1269
 *
 * Strategy:
 * Aho-Corasick.
 * 
 * Performance:
 * Linear in the size of the input, runs the tests in 0.468s using 6,740KB memory.
 */

#include <iostream>
#include <string>
#include <limits>
#include <queue>
#include <unordered_map>

int inf = 100000000;

const int TRIE_LEN = 30000;

#pragma pack (push, 1)
struct node {
    std::unordered_map<char, unsigned short> v;
    unsigned short suf;
    short d;

    node() : d(-1) {}
} trie[TRIE_LEN];
#pragma pack(pop)

int t = 1;

void addWord(std::string& str) {
    // Adds a word to the trie
    node* n = trie;
    for(char c : str) {
        if(n->v.find(c) == n->v.end())
            n->v[c] = t++;
        n = &trie[n->v[c]];
    }
    n->d = str.size();
}

void addSuf() {
    // Adds suffix links to the trie
    std::queue<int> q({ 0 });
    int n;

    for(int i = 0; i < t; i++)
        trie[i].suf = 0;

    while(!q.empty()) {
        n = q.front();
        q.pop();
        for(const auto& [c, v] : trie[n].v) {
            for(int suf = trie[n].suf; ; suf = trie[suf].suf) {
                if(trie[suf].v.find(c) != trie[suf].v.end() && trie[suf].v[c] != v) {
                    trie[v].suf = trie[suf].v[c];
                    break;
                }
                if(suf == trie[suf].suf)
                    break;
            };
            if(trie[v].d < trie[trie[v].suf].d)
                trie[v].d = trie[trie[v].suf].d;
            q.push(v);
        }
    }
}

int find(std::string& str) {
    node* n = trie;

    int mini = inf;

    for(int i = 0; i < str.size(); i++) {
        char c = str[i];

        while(n->v.find(c) == n->v.end() && n != trie)
            n = &trie[n->suf];
        if(n->v.find(c) != n->v.end())
            n = &trie[n->v[c]];
        if(n->d != -1)
            mini = std::min(mini, 1 + i - n->d);
    }
    return mini;
}

std::pair<int, int> solve(std::vector<std::string>& lines) {
    for(int i = 0; i < lines.size(); i++) {
        auto& str = lines[i];
        int j = find(str);
        if(j != inf)
            return { i+1, j+1 };
    }
    return { inf, inf };
}

void clear() {
    for(auto& node : trie) {
        node.v.clear();
        node.d = -1;
        node.suf = 0;
    }
    t = 1;
}

int main() {
    int n;
    std::pair<int, int> sol = { inf, inf };
    std::string str;
    std::vector<std::string> words;
    std::vector<std::string> lines;

    auto getN = [&n] {
        std::cin >> n;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
    };

    getN();
    for(int i = 0; i < n; i++) {
        std::getline(std::cin, str);
        words.push_back(str);
    }

    getN();
    for(int i = 0; i < n; i++) {
        std::getline(std::cin, str);
        lines.push_back(str);
    }

    // Actually run the algorithm several times because we have a bit of a memory problem.
    // Putting the unordered_map in the tree nodes as a global variable might have helped
    // as well.
    for(int i = 0; i < words.size();) {
        for(int size = 0; i < words.size() && size + words[i].size() < TRIE_LEN; i++) {
            size += words[i].size();
            addWord(words[i]);
        }

        addSuf();
        auto s = solve(lines);
        if(s != std::make_pair(inf, inf))
            sol = std::min(s, sol);

        clear();
    }

    if(sol != std::make_pair(inf, inf)) {
        std::cout << sol.first << " " << sol.second;
        return 0;
    }
    
    std::cout << "Passed";
}




