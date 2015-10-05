/* 1094. E-screen - http://acm.timus.ru/problem.aspx?num=1094
 *
 * Strategy:
 * N/A ("Difficulty: 402"!?)
 *
 * Performance:
 * Linear in the size of the input, runs in 0.001 using 176KB memory.
 */

#include <stdio.h>
#include <cstring>
#include <algorithm>

char buf[81], p = 0;

int main()
{
    std::memset(buf, ' ', 80);
    while(true)
    {
        char c = getc(stdin);
        if(c == '\n' || c == '\r' || c == EOF)
            break;
        if(c == '<')
            p = std::max(0, p-1);
        else if(c == '>')
            p = (p + 1) % 80;
        else
            buf[p] = c, p = (p + 1) % 80;
    }
    printf("%s", buf);
    return 0;
}