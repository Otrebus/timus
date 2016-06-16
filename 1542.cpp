/* 1542. Autocompletion - http://acm.timus.ru/problem.aspx?num=1542
 *
 * Strategy:
 * Build a trie in which every node stores the ten most frequent (n_i given in the problem
 * description) strings contained in the subtree of that node. To save memory, the trie constructed
 * in two passes. First the query strings are used to build the topology of the tree in the normal
 * way, and in the second pass the tree is searched using the full words, and in the search path
 * for each word we update the frequencies for the nodes encountered, calculating the vector of
 * the most frequent strings in each node as described above.
 *
 * Performance:
 * O(n log n + m), runs the test cases in 0.202s using 37,260KB memory.
 */

#include <stdio.h>
#include <vector>
#include <algorithm>
#include <cstring>

const int maxn = 100000, maxm = 15000, maxl = 15;

struct Word
{
    int value; // Frequency
    char str[15+1];
} words[maxn]; // The input words

char queries[maxm][maxl+1]; // The query strings

struct Node
{
    std::vector<Word*> top; // The most frequent words of this subtree
    Node* v[1+'z'-'a'];
} nodes[maxm*maxl], *pn = nodes+1; // The trie

// Searches the tree for this word while initializing the top vector; terminates whenever either
// the full word was found or when a leaf was reached. Assumes the word input has the highest
// encounter frequency out of all the remaining words to be inserted
void insert(Word* w, int i, Node* n)
{
    auto& slot = n->v[w->str[i]-'a'];
    if(n->top.size() < 10)
        n->top.push_back(w); // This is one of the biggest strings of this subtree
    if(!w->str[i]) // The reserve function (below) only got this far
        return;
    if(slot)
        insert(w, i+1, slot);
}

// Expands the trie with the given input string
void reserve(Node* node, char* input)
{
    auto& slot = node->v[*input-'a'];
    if(!*input)
        return;
    if(!slot)
        slot = pn++;
    reserve(slot, input+1);
}

// Prints the output for this query as per the problem description
void print(Node* node, char* input)
{
    if(!*input || !node->v[*input-'a'])
        for(auto t : node->top)
            printf("%s\n", t->str);
    else
        print(node->v[*input-'a'], input+1);
}

int main()
{
    int N, M;
    scanf("%d", &N);
    for(int i = 0; i < N; i++)
        scanf("%s %d", words[i].str, &words[i].value);
    std::vector<Word*> wp; // Pointers to these words, for faster sorting
    for(int i = 0; i < N; i++)
        wp.push_back(words+i);
    // We insert the words with the biggest encounter frequency first to avoid having to sort stuff
    // in the insertion function, so we sort this vector
    std::sort(wp.begin(), wp.end(), [] (Word* a, Word* b) 
          { return a->value == b->value ? strcmp(a->str, b->str) < 0 : a->value > b->value; } );
    scanf("%d", &M);
    for(int i = 0; i < M; i++)
    {
        char input[16];
        scanf("%s", queries[i]);
        reserve(nodes, queries[i]); // Expand the trie with this query word
    }
    for(int i = 0; i < wp.size(); i++)
        insert(wp[i], 0, nodes); // Update the top frequencies for the existing nodes in the branch
                                 // corresponding to this word
    for(int i = 0; i < M; i++)
    {
        print(nodes, queries[i]);
        printf(i < M-1 ? "\n" : "");
    }
}