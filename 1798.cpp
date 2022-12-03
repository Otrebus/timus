/* 1798. Fire Circle. Version 2 - http://acm.timus.ru/problem.aspx?num=1798
 *
 * Strategy:
 * Naive.
 *
 * Performance:
 * O(r), runs the test cases in 1.562s using 160KB memory.
 */

#include <stdio.h>
#include <stdlib.h>

void main()
{
    long long r;
    scanf("%lld", &r);

    long long y = r, a = 0;
    for(long long x = 0; x < r; x++) {
        while(y*y >= r*r - x*x)
            y--;
        a += y + 1;
    }
    printf("%lld", a*4);
}
