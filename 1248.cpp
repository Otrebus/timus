/* 1248. Sequence Sum - http://acm.timus.ru/problem.aspx?num=1248
 *
 * Strategy:
 * Straightforward calculation.
 *
 * Performance:
 * Roughly linear in the size of the input, runs in 0.015s using 172KB memory.
 */

#include <cstring>
#include <stdio.h>

const int w = 250;
char acc[w], dummy; // Accumulator for the answer, dummy for shorter code
char add[w]; // Term for each of the n lines
char* u = add+w/2, *v = acc+w/2; // To simplify some notation and allow negative indexing

int main()
{
    int n;
    scanf("%d", &n);

    while(n--)
    {
        std::memset(add, 0, sizeof(add));
        char s[50], num[50], *p; // Num will contain the mantissa except the dot
        scanf("%s", s);
        p = s;
        int pos = 0, exp;
        while(*p != 'e') // Read the mantissa into num
            *((*p != '.') ? num + pos++ : &dummy) = *p++ - '0'; // Read into dummy if '.', else num
        num[pos] = -1;
        sscanf(p+1, "%d", &exp); // Read the exponent
        for(p = num; *p >= 0; p++) // Move the mantissa to the correct position given exp
            u[exp--] = *p;
        for(int i = 0; i < w-1; i++) // Add the term to the answer
        {
            acc[i] += add[i];
            acc[i+1] += acc[i]/10;
            acc[i] %= 10;
        }
    }
    int maxdigit;
    for(maxdigit = w/2-1; !v[maxdigit]; maxdigit--);
    printf("%d.", v[maxdigit]);
    for(int e = maxdigit-1; e >= maxdigit-19; e--)
        printf("%d", v[e]);
    printf("e%d\n", maxdigit);
}