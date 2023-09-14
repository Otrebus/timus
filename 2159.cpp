/* 2159. Essay - http://acm.timus.ru/problem.aspx?num=2159
 *
 * Strategy:
 * Construct a directed graph starting from the n required words and perform a version of
 * topological sorting where we maintain a set of visitable nodes (nodes with no unencountered
 * ingoing edges) ordered lexicographically, which we initialize to all the n required keywords
 * without ingoing edges.
 *
 * Performance:
 * O((n+k)log (n+k)), runs the tests in 0.484s using 38,312KB memory.
 */

#include <vector>
#include <iostream>
#include <string>
#include <map>
#include <algorithm>
#include <set>

#pragma comment(linker, "/STACK:16777216")

struct node {
    std::string word; // The word represented by the node
    std::vector<int> v; // Outoing edges
    bool b, s; // Visited in dfs, on stack
    int in, rank; // Ingoing required edges, rank
} nodes[200000];

int m = 0; // Total number of edges
std::map<std::string, int> M; // Maps words to edges
int B[200000]; // Used to rank edges
std::set<std::pair<int, int>> S; // Reachable nodes ordered by rank
std::vector<std::string> out; // Output

bool dfs(int i) { // Detects cycles and counts indegrees
    if(nodes[i].b)
        return true;
    nodes[i].s = true;
    nodes[i].b = true;
    for(auto j : nodes[i].v) {
        if(nodes[j].s)
            return false;
        nodes[j].in++;
        if(!dfs(j))
            return false;
    }
    nodes[i].s = false;
    return true;
}

void addWord(std::string& a) { // Adds a node with a word to the graph
    nodes[m].word = a;
    M[a] = m++;
}

int main() {
    int n, k;
    std::string a, b;
    std::cin >> n >> k;

    for(int i = 0; i < n; i++) {
        std::cin >> a;
        addWord(a);
    }

    for(int j = 0; j < k; j++) {
        std::cin >> a >> b;
        if(M.find(a) == M.end())
            addWord(a);
        if(M.find(b) == M.end())
            addWord(b);
        nodes[M[a]].v.push_back(M[b]);
    }

    // Rank nodes by words
    for(int i = 0; i < m; i++)
        B[i] = i;
    std::sort(B, B+m, [] (int i, int j) { return nodes[i].word < nodes[j].word; });
    for(int i = 0; i < m; i++)
        nodes[B[i]].rank = i;

    for(int i = 0; i < n; i++) // Detect cycles, add indegrees
        if(!dfs(i))
            return (std::cout << -1, 0);

    for(int i = 0; i < n; i++) // Add all start nodes to S
        if(!nodes[i].in)
            S.insert({nodes[i].rank, i});

    // Pop nodes from S and add reachable nodes to it
    while(!S.empty()) {
        auto it = S.begin();
        auto [_, i] = *it;
        S.erase(it);
        out.push_back(nodes[i].word);
        for(auto j : nodes[i].v)
            if(nodes[j].in-- == 1) // No more required in-edges
                S.insert({ nodes[j].rank, j });
    }

    for(int i = 0; i < out.size(); i++)
        std::cout << out[i] << (i < out.size()-1 ? " " : "");
}
