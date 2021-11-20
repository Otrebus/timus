/* 1532. Lost in Translation - http://acm.timus.ru/problem.aspx?num=1532
 *
 * Strategy:
 * Build a trie with the given strings, and for each string do a dfs through the trie with the
 * string as input, while allowing two mistakes of the allowed types (skipping a transition in the
 * trie while advancing the character in the string, the opposite, or taking a different transition
 * than the one specified by the string), and see if we end up in an existing node in the trie.
 *
 * Performance:
 * O(K) (with a huge constant), runs the tests in 3.406s using 10,992KB memory.
 */

#include <stdio.h>
#include <algorithm>
#include <vector>
#include <cstring>

char T[5001][16]; // The input strings
char* S[5001]; // Pointers to the input strings (allows for sorting)
int n, i;
bool A[5001]; // Input strings part of the answer

struct node {
    std::vector<std::pair<char, int>> v; // Outgoing character in the trie
    int out[1+'z'-'a']; // Outgoing character in the trie
    int i; // Input string i ends here
} nodes[15*5001+1];
int p = 1;

int add(int i, char* c, int q) {
    // Adds a string with index i and current character c into trie at node p
    if(!*c)
        return nodes[q].i = i;

    // If the character already exists as an outgoing char in this trie node, recurse
    int x = nodes[q].out[*c-'a'];
    if(x)
        return add(i, ++c, x);

    // Otherwise, add it and recurse
    nodes[q].out[*c-'a'] = ++p;
    nodes[q].v.push_back({*c, p});
    add(i, ++c, p);
}

void dfs(char* c, int p, int f) {
    // Checks string starting at char c into trie node p, having made f mistakes 
    if(f > 2 || !p) // Disallowed transition
        return;

    // No more letters in the string left, we made some mistakes and some string in the
    // trie ended here, add it to the answer
    if(!*c && f && nodes[p].i && nodes[p].i != i)
        A[nodes[p].i] = true;

    for(auto pr : nodes[p].v) { // For each outgoing character in the trie
        char ch = pr.first; int q = pr.second;
        if(*c)
            dfs(c+1, q, f+(ch!=*c)); // Replace the input char with the one in the trie
        dfs(c, q, f+1); // Skip the char in the trie
    }
    if(*c)
        dfs(c+1, p, f+1); // Skip the char in the string
}

int main() {
    int K;
    scanf("%d", &K);
    for(int i = 1; i <= K; i++) // Read input
        scanf("%s", T[i]), S[i] = T[i];

    // Sort the input strings
    std::sort(S+1, S+K+1, [] (const char* a, const char *b) { return strcmp(a, b) < 0; });

    // Add the strings into the trie
    for(i = 1; i <= K; i++)
        add(i, S[i], 1);

    // For each input string, trace it through the trie while allowing mistakes and see what we add
    for(i = 1; i <= K; i++)
        dfs(S[i], 1, 0);

    // Count and output the answer strings
    int s = 0;
    for(int i = 1; i <= K; i++)
        s += A[i];
    printf("%d\n", s);
    for(int i = 1; i <= K; i++)
        if(A[i])
            printf("%s\n", S[i]);
}
