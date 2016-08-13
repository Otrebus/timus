/* 1780. Gray Code - http://acm.timus.ru/problem.aspx?num=1780
 *
 * Strategy:
 * Consider the iterative way to build a gray code from a given index; if G_i and I_i are the bits
 * of the Gray code and index, respectively, then G_i = I_i ^ I_{i-1}. For a given index and gray
 * code with "unreadable" digits, a slow solution would be to read both strings from left to right
 * and at each question mark, split and do a depth-first search and for every branch check for
 * compatibility once all digits have been read. A faster way is to use dynamic programming where we
 * maintain the array A[k][d] that stores the total number of possible matches up to inputs of
 * length k where the last digit of the input was set to d. We update that array from k = 1 to
 * k = n, at each iteration using the previous values at A[k-1] and the inputs at G[k] and I[k].
 *
 * Performance:
 * Linear in the size of the input, runs the tests in 0.015s using 560KB memory.
 */

#include <stdio.h>
#include <algorithm>

const int maxn = 100000;

char A[maxn+1][2] = {1}; // A[0][0] = 0 essentially means that some arbitrary prefix of only zeroes
                         // corresponds to a gray code consisting of only zeroes
char index[maxn+2], gray[maxn+2];

// Converts an index to a gray code
void togray(char* gr, char* ind, int n)
{
    for(int i = 0, prev = 0; i < n; i++)
    {
        char x = ind[i] - '0';
        gr[i] = '0' + (x ^ prev);
        prev = x;
    }
}

int main()
{
    int n;
    scanf("%s%n%s", index+1, &n, gray+1);
    for(int i = 1; i <= n; i++)
    {
        char x = index[i]-'0', y = gray[i]-'0';
        // Given the current digits in the index and gray code, calculate the number of different
        // solutions we have for the subcodes read so far by using the solutions for the strings
        // that are one digit shorter
        if(index[i] == '?')
            if(gray[i] == '?')
                A[i][1] = A[i][0] = A[i-1][0] + A[i-1][1];
            else
                A[i][1] = A[i-1][y^1], A[i][0] = A[i-1][y];
        else
            if(gray[i] == '?')
                A[i][x] = A[i-1][0] + A[i-1][1];
            else
                A[i][x] = A[i-1][y^x];
        // We clamp the actual values since we only need to know if there's more than one
        A[i][1] = std::min(A[i][1], (char) 2);
        A[i][0] = std::min(A[i][0], (char) 2);
    }
    if(A[n][1] + A[n][0] == 0)
        printf("Impossible");
    else if(A[n][1] + A[n][0] > 1)
        printf("Ambiguity");
    else
    {
        // Recreate a matching index by reading the solution array backwards and comparing it
        // with the input at each step to select a new compatible previous index digit
        int d = A[n][1] ? '1' : '0';
        for(int i = n; i; i--)
        {
            index[i] = d;
            if(gray[i] == '?')
                d = A[i-1][1] ? '1' : '0';
            else
                d = '0' + (gray[i] != d);
        }
        togray(gray+1, index+1, n);
        printf("%s\n%s", index+1, gray+1);
    }
}



