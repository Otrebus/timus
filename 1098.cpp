/* 1098. Questions - http://acm.timus.ru/problem.aspx?num=1098
 *
 * Strategy:
 * Build a heap-like tree on top of the string that keeps track of the number of letters in each
 * subheap, like so (for the input "QUESTION"):
 *
 *                                             5
 *                                         2       3
 *                                       1   1   2   1 
 *                                      1 0 1 0 1 1 0 1
 *                                      Q U E S T I O N
 *
 * With this added information, we can quickly find the kth remaining letter in the string, remove
 * it, and update the heap.
 *
 * Performance:
 * Linearithmic in the size of the input, runs the test set in 0.015s using 456KB memory.
 */

#include <stdio.h>

int sz, p, N = 1999, i = 0, pos = 0;
char str[30001];
int A[65537]; // The heap

void iterate()
{
    pos = (pos + N-1) % p;
    int newPos = pos;

    // Find the leaf of the position in the tree
    for(i = 1; i < sz/2; )
        if(A[i*2]-1 < newPos)
            newPos -= A[i*2], i = i*2 + 1;
        else
            i = i*2;
    // Update the leaf and its parents
    A[i] = 0;
    for(int j = i/2; j > 0; j /= 2)
        A[j] = A[j*2]+A[j*2+1];
    // Go to the next position
    pos = pos % (p > 1 ? p-1 : 1);
}

int main()
{
    // Read input
    for(char c = getc(stdin); c != EOF; c = getc(stdin))
        if(c == '\n' || c == '\r')
            continue;
        else
            str[p++] = c;
    str[p] = 0;

    // Find the right size of the heap. The number of leaves is the first power of 2 above the
    // length of the string, and we need twice that
    for(sz = 1; sz*2 < p*4; sz *= 2);
    // Initialize all leaves to 1 and the rest of the tree nodes to the sum of its children
    for(int i = sz/2; i < sz; i++)
        A[i] = 1;
    for(int i = sz/2-1; i > 0; i--)
        A[i] = A[i*2] + A[i*2+1];

    // Remove letters ...
    do
        iterate();
    while(--p);

    // The last leaf where we end up holds the answer. In the heap, this is at position 1+sz/2
    if(str[i-sz/2] == '?')
        printf("Yes\n");
    else if(str[i-sz/2] == ' ')
        printf("No\n");
    else
        printf("No comments\n");
    return 0;
}