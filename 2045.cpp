/* 2045. Richness of words - http://acm.timus.ru/problem.aspx?num=2045
 *
 * Strategy:
 * Given and n and i's, the strings of length n > 2 of the form a{i-2}(bca)*(b(c(a?)?)? satisfy the
 * requirements given. Special cases are few and handled separately.
 * 
 * Performance:
 * O(n^2), runs in 0.015s using 204KB memory.
 */

#include <stdio.h>
#include <algorithm>

char a[2001];
char* x = "bca";

bool printAns(int k, int n)
{
    int p = 0, c = 0;
    if(k > 2)
    {
        for(int i = 0; i < std::min(k-2, n); i++)
            a[p++] = 'a';
        for(int i = k-2; i < n; i++)
            a[p++] = x[c++%3];
        a[p] = 0;
        return true;
    }
    else if(n == k && k < 3)
    {
        for(int i = 0; i < k; i++)
            a[p++] = 'a';
        a[p] = 0;
        return true;
    }
    return false;
}

int main()
{
    int n;
    scanf("%d", &n);
    for(int i = 1; i <= n; i++)
    {
        bool b = printAns(i, n); 
        printf("%d : %s\n", i, (b ? a : "NO"));
    }
}