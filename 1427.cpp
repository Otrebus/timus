/* 1427. SMS - http://acm.timus.ru/problem.aspx?num=1427
 *
 * Strategy:
 * Greedy - scan the input and keep sending the longest possible SMS.
 *
 * Performance:
 * Linear in the size of the input, runs in 0.015s using 284KB memory (rated 47/970).
 */

#include <stdio.h>

char input[100001];

inline bool isAlpha(const char c)
{
    return (c >= 'A' || c == ' ');
}

int main()
{
    int N, M;
    gets(input);
    sscanf(input, "%d %d", &N, &M);
    gets(input);

    int ans = 0, sz = 0; // sz is the size of the current sms
    bool allAlpha = true;
    for(char* c = input; *c; c++)
    {
        allAlpha &= isAlpha(*c);
        if(!allAlpha && sz >= N || allAlpha && sz >= M)
        {
            ans++;
            sz = 0;
            allAlpha = isAlpha(*c);
        }
        sz++;
    }
    printf("%d", ans + bool(sz));
}