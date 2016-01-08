/* 1322. Spy - http://acm.timus.ru/problem.aspx?num=1322
 *
 * Strategy:
 * Burrows-Wheeler inverse transform.
 *
 * Performance:
 * Linear, runs the tests in 0.015s using 1496KB memory.
 */

#include <stdio.h>
#include <vector>

std::vector<int> s[256];
char in[100001];
int n[100000], f, p = 0;

int main()
{
    scanf("%d %s", &f, in);
    for(int i = 0; in[i]; i++)
        s[in[i]].push_back(i);
    for(int i = 0; i < 256; i++)
        for(auto q : s[i])
            n[p++] = q;
    for(int i = 0, x = f-1; i < p; i++)
        putchar(in[x = n[x]]);
}