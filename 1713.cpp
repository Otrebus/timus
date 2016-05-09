/* 1713. Key Substrings - http://acm.timus.ru/problem.aspx?num=1713
 *
 * Strategy:
 * For each substring length n, go through all commands and add all substrings of that length
 * into a hash structure (using a rolling hash to speed things up). For each command, check if
 * any of its substrings have only been added by that command. In that case, that substring is
 * a "key" substring of that command.
 *
 * Performance:
 * O(nm^3), runs the test cases in 1.248s using 4,852KB memory.
 */

#include <unordered_map>
#include <stdio.h>
#include <cstring>

const int maxn = 1000, maxm = 100;
char A[maxn][maxm+1];
const int base = 101797;

class StringHash
{
    struct Node
    {
        Node* next; // For collisions
        short word; // The # of the command that added this string
        bool unique; // Flagged false if this is added by several commands
        std::pair<char, char> seq; // The position of the substring in the command
    } nodes[maxn*maxm];
    int head = 0;
    std::unordered_map<unsigned int, Node*> map;

public:
    void insert(short word, char a, char b, unsigned int hash)
    {
        if(map.find(hash) != map.end())
        {
            for(Node* n = map[hash]; n; n = n->next)
            {
                if(b-a == n->seq.second-n->seq.first // Manual string comparison
                   && strncmp(A[n->word]+n->seq.first, A[word]+a, b-a) == 0)
                {
                    if(n->word != word) // If this was added by another cmd, flag as non-keyword
                        n->unique = false;
                    return;
                }
            }
        }
        map[hash] = &(nodes[head++] = { map[hash], word, true, { a, b } });
    }

    void populate(std::pair<char, char>* pairs)
    { // Uses the information gathered to assign commands their keys
        for(auto p : map)
            for(auto n = p.second; n; n = n->next)
                if(n->unique && pairs[n->word].first == -1)
                    pairs[n->word] = n->seq;
    }

    void clear()
    {
        map.clear();
        head = 0;
    }
} sh;

void addAll(int n, int len)
{
    if(strlen(A[n]) < len)
        return;
    unsigned int hash = 0;
    unsigned int p = 1; // Powered base
    
    for(int i = len-1; i >= 0; i--)
    {
        hash += A[n][i]*p;
        p *= base;
    }
    for(int i = 0; A[n][i+len-1]; i++)
    {
        sh.insert(n, i, i+len, hash);
        hash *= base; // Roll the hash
        hash -= A[n][i]*p;
        hash += A[n][i+len];
    }
}

std::pair<char, char> ans[1000];

int main()
{
    int n;
    scanf("%d", &n);
    for(int i = 0; i < n; i++)
    {
        ans[i] = { -1, -1 };
        scanf("%s", A[i]);
    }
    for(int len = 1; len <= maxm; len++)
    {
        for(int j = 0; j < n; j++)
            addAll(j, len);
        sh.populate(ans);
        sh.clear();
    }
    for(int i = 0; i < n; i++)
    {
        for(int j = ans[i].first; j < ans[i].second; j++)
            putchar(A[i][j]);
        putchar('\n');
    }
}