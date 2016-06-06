/* 1238. Folding - http://acm.timus.ru/problem.aspx?num=1238
 *
 * Strategy:
 * Dynamic programming. Let the optimal solution for a string S be denoted P(S). For some given 
 * S and for every partition S = AB where A and B are substrings, P(S) <= min { P(A) + P(B) } = m
 * where the minimum is taken over every possible partition into A and B. We also check if S
 * consists of entirely repeatable characters, and in that case we choose the smallest 
 * representation of those repeatable characters. If this repetition representation is of size M,
 * the solution becomes P(S) = min(m, M). This solution is constructed bottom-up, starting with
 * strings of size 1, up to n, and all subsolutions are represented by { partition location, length
 * of the solution }, for every substring.
 *
 * Performance:
 * O(N^3), executes the test suite in 0.015s using 256KB memory.
 */

#include <stdio.h>
#include <cstring>
#include <string>
#include <algorithm>

struct answer
{
    char i; // Partition location
    char l; // Length of the subsolution
};

char A[101]; // The input string
int rep[101][101]; // rep[i][k] = c means the string of length k at position i is repeated c times
answer ans[101][101]; // The optimal answer for 

int len(int n) // Number of digits in the number n < 1000
{
    return n < 10 ? 1 : n < 100 ? 2 : 3;
}

void printAns(char* str, int i, int k) // Prints the solution of length k at position i to str
{
    answer a = ans[i][k];
    if(!k)
        return;
    else if(k == 1)
        sprintf(str, "%c", A[i]);
    else if(a.i < 0) // Indicates repetition, in this case -a.i is the length of the repeated string
    {
        sprintf(str, "%d(", (k/-a.i));
        printAns(str + len(k/-a.i) + 1, i, -a.i);
        sprintf(str + len(k/-a.i) + 1 +ans[i][-a.i].l, ")");
    }
    else // String was split at location a.i
    {
        printAns(str, i, a.i-i);
        printAns(str+ans[i][a.i-i].l, a.i, k-(a.i-i));
    }
}

int main()
{
    scanf("%s", A);
    int n = strlen(A);
    for(int k = 1; k <= n; k++)
        for(int i = n-k; i >= 0; i--) // Builds information about repeatable characters
            rep[i][k] = (i + k < n && strncmp(A+i, A+i+k, k) == 0 ? rep[i+k][k] : 0) + 1;
    
    for(int i = 0; i <= n; i++)
        ans[i][1].l = 1, ans[i][1].i = i; // Base case for strings of length 1
    for(char k = 2; k <= n; k++) // For each length k of string ..
    {
        for(char i = 0; i <= n - k; i++) // .. starting at position i in A
        {
            answer bestans = { i, 110 };
            for(char j = i+1; j < i + k ; j++) // Try each split at positions i < j < i + k
                if(ans[i][j-i].l + ans[j][k-(j-i)].l < bestans.l)
                    bestans = { j, ans[i][j-i].l + ans[j][k-(j-i)].l };
            for(char l = 1; l < k; l++) // Check if the string was all repeatable characters
                if(std::min(rep[i][l], k/l)*l == k 
                       && len(std::min(rep[i][l], k/l)) + 2 + ans[i][l].l < bestans.l)
                    bestans = { -l, len(std::min(rep[i][l], k/l)) + 2 + ans[i][l].l };
            ans[i][k] = bestans;
        }
    }
    char str[101];
    printAns(str, 0, n);  // Print the string to an
    str[ans[0][n].l] = 0; // Zero-terminate
    printf("%s\n", str);
}