/* 1414. Astronomical Database - http://acm.timus.ru/problem.aspx?num=1414
 *
 * Strategy:
 * Trie.
 * 
 * Performance:
 * Linear in the input size, runs the test suite in 0.031s using 23488KB memory (rated 2/738).
 */

#include <stdio.h>
#include <stack>

const int maxq = 10000, maxw = 20;
const int nchars = 10+1+'z'-'a';

struct Node
{
    Node* v[nchars]; // Children
    char ch; // The character that this node represents
    bool word; // Is this the last letter in a word?
} nodes[maxq*maxw+4];
int nNode = 1;

// Maps a character into its child index, '0'-'9' -> 0-9, 'a'-'z' -> 10-35
int getIndex(char c)
{
    int offset = 0, first = '0';
    if(c >= 'a' && c <= 'z')
        offset += 10, first = 'a';
    return offset+c-first;
}

// Adds a name to the trie
void addName(char* name)
{
    Node* cur = nodes;
    for(char* c = name; *c; c++)
    {
        int index = getIndex(*c);
        if(!cur->v[index])
            cur->v[index] = &nodes[nNode++];
        cur->v[index]->ch = *c;
        cur = cur->v[index];
    }
    cur->word = true;
}

void printNames(char* pre)
{
    printf("%s\n", pre);
    std::stack<Node*> s; // For the dfs through the trie
    Node* root = nodes;
    int found = 0, pos = 0, i = 0;
    char str[21];

    // str contains the string we build as we go through the tree, pos is the last char
    for(; pre[i]; i++)
        str[i] = pre[i];
    for(pos = i-1; i <= 20; i++)
        str[i] = 0;
    
    // Find the root node of the given string
    while(*pre)
    {
        int i = getIndex(*pre);
        if(root->v[i])
            root = root->v[i];
        else
            return;
        pre++;
    }
    s.push(root);
    // Dfs from there
    while(!s.empty() && found < 20)
    {
        auto node = s.top();
        s.pop();
        if(!node) // This node is done, we move towards the root, so pop one character
            str[--pos] = 0;
        else
        {
            str[pos++] = node->ch;
            if(node->word)
            {
                printf("  %s\n", str);
                found++;
            }
            s.push(0); // When popped, this signifies we are done with this node
            for(int i = nchars-1; i >= 0; i--)
                if(node->v[i])
                    s.push(node->v[i]);
        }
    }
}

int main()
{
    char input[100];
    nodes[0].word = true;
    addName("sun");
    while(gets(input))
    {
        if(input[0] == '?')
        {
            int i = 1;
            while(input[i] == ' ') // Skip whitespace
                i++;
            printNames(input+i);
        }
        else
        {
            int i = 1;
            while(input[i] == ' ')
                i++;
            addName(input+i);
        }
    }
}