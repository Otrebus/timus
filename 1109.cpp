/* 1107. Warehouse Problem - http://acm.timus.ru/problem.aspx?num=1107
 *
 * Strategy:
 * The difference of the sums of any two similar sets must be between 1 and N (since all sets are
 * unique) so we use the modulus of the sum of each set to determine the number of the shop.
 *
 * Performance:
 * Linear in the size of the input, runs in 0.343s using 184KB memory.
 */

#include <stdio.h>
#include <cctype>

inline int getint()
{
    int ret = 0, ch;
    while(!isdigit(ch = getc(stdin)));
    ret = ch - '0';
    while(isdigit(ch = getc(stdin)))
        ret = ret*10 + ch - '0';
    return ret;
}

int main()
{
    int N = getint(), K = getint(), M = getint();
   
    printf("YES\n");
    for(int i = 0; i < K; i++)
    {
        int n = getint();
        int sum = 0;
        while(n--)
            sum += getint();
        printf("%d\n", 1 + sum % (N+1));
    }
    return 0;
}