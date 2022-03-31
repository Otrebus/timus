/* 2040. Palindromes and Super Abilities 2 - http://acm.timus.ru/problem.aspx?num=2040
 *
 * Strategy:
 * We use a tree similar to Aho-Corasick where we store palindromes instead of full strings, into
 * which we insert the letters one by one and report '1' if a new node was created.
 *
 * Performance:
 * O(n), runs the tests in 0.171s using 88,016KB memory.
 */

#include <stdio.h>
#include <tuple>

struct node {
    int out[2]; // Outgoing links for 'a' and 'b'
    int l, s; // Length, link to suffix node
} nodes[5000003];

int t = 2; // Current node
char s[5000003], out[5000003];
int n, i;

std::pair<node*, bool> add(char c, node* n) { // Adds a character c to the node n in the tree
    if (n->out[c-'a']) // Existing node was found
        return { &nodes[n->out[c-'a']], false };

    n->out[c - 'a'] = t++;
    auto n2 = &nodes[n->out[c-'a']];
    n2->l = n->l + 2;

    // Find the suffix link by running along the links and finding one that fits
    if (n2->l > 1) {
        auto e = &nodes[n->s];
        // As long as the suffix node doesn't match (or it's an even root that doesn't)
        while (!e->out[c-'a'] || s[i-e->l-1] != s[i]) // .. run up the suffix links
            e = &nodes[e->s];
        n2->s = e->out[c-'a'];
    }
    else if (n2->l == 1) // Single characters have even root nodes as suffix link
        n2->s = 1;       // to help check thoroughly
    return { n2, true };
}

int main() {
    scanf("%s%n", s + 1, &n);

    nodes[0].l = -1;
    nodes[1].l = 0;
    node* cur = &nodes[0];

    for (i = 1; i <= n; i++) {
        // Until we find a matching node in the tree, walk down the suffix links
        while (s[i-cur->l-1] != s[i])
            cur = &nodes[cur->s];
        // Expand the tree
        auto p = add(s[i], cur);
        cur = p.first;
        out[i] = p.second ? '1' : '0';
    }
    printf("%s", out + 1);
}
