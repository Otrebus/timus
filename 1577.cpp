/* 1577. E-mail - http://acm.timus.ru/problem.aspx?num=1577
 *
 * Strategy:
 * Dynamic programming. Let A[i][j] be the solution for the substrings that end at indices i and j.
 * We can reach A[i][j] by either adding the last character from the first or second substrings, or
 * if the previous two substrings match, both at the same time. We keep track of the length of the
 * solution and make sure we only propagate the solution with the shortest number of characters.
 *
 * Performance:
 * Quadratic in the size of the input, runs the tests in 0.031s using 31576KB memory (rated 6/608).
 * Less memory can be used through the realization that we only use data from the current and
 * previous outer iterations of the loop.
 */

#include <stdio.h>
#include <cstring>

struct { short chars; int ans; } A[2002][2002];
char s1[2002], s2[2002];

inline void update(int i1, int j1, int i2, int j2)
{
    auto& a = A[i1][j1], &b = A[i2][j2];
    if(a.chars + 1 < b.chars)
        b = { a.chars + 1, a.ans };
    else if(a.chars + 1 == b.chars)
        b = { a.chars + 1, (a.ans + b.ans) % 1000000007 };
}

int main()
{
    scanf("%s %s", s1+1, s2+1);
    std::memset(A, 0x7F, sizeof(A)); 
    A[0][0] = { 0, 1 }; 
    s1[0] = s2[0] = '$';
    int lena = strlen(s1), lenb = strlen(s2);
    for(int i = 1; i <= lena; i++)
        for(int j = 1; j <= lenb; j++)
            if(s1[i-1] == s2[j-1])
                update(i-1, j-1, i, j);
            else
                update(i-1, j, i, j), update(i, j-1, i, j);
    printf("%d\n", A[lena][lenb].ans);
}