/* 1960. Palindromes and Super Abilities - http://acm.timus.ru/problem.aspx?num=1960
 *
 * Strategy:
 * Palindromic tree.
 *
 * Performance:
 * O(n), runs the test cases in 0.046s using 11244KB memory (rated 13/214).
 */

#include <stdio.h>
#include <cstring>

const int maxn = 100000;

struct Node
{
    Node* out[1 + 'z'-'a'];
    Node* next; // Link to the longest palindromic suffix of this palindrome
    int width; // Size of this palindrome
} nodes[maxn];

Node* r1[1 + 'z'-'a'];
Node* r2[1 + 'z'-'a'];
int len, n, c;
Node* head;
char str[maxn+1];

void jumpTo(Node**& cur, Node*& newNode, int width) // Updates the suffix links of the tree
{
    if(!newNode)
        newNode = &nodes[n++];
    *cur = newNode; // The ->next of the previous node, or head, will now point to this new node
    cur = &(newNode->next); // The new cur points to the ->next of this node
    newNode->width = width;
}

int addChar()
{
    Node** cur = &head; // cur points at the ->next of the previous node, which points to this node
    char ch = str[c]-'a';
    while(true)
    {
        Node* node = *cur;
        if(node == 0) // Special case for the root vertex
        {
            if(c > 0 && str[c] == str[c-1])
                jumpTo(cur, r2[ch], 2);
            jumpTo(cur, r1[ch], 1);
            break;
        }
        else if(c-node->width > 0 && str[c] == str[c-node->width-1]) // This suffix is a palindrome
        {
            if(node->out[ch]) // If we already had a node representing this palindrome suffix, break
            {
                *cur = node->out[ch];
                break;
            }
            else // Otherwise, add it and recurse
            {
                jumpTo(cur, node->out[ch], node->width + 2);
                node->out[ch]->next = node->next;
            }
        }
        else
            *cur = (*cur)->next;
    }
    c++;
    return n;
}

int main()
{
    scanf("%s", str);
    len = strlen(str);
    while(len--)
        printf("%d ", addChar());
}