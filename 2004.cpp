/* 2004. Scientists from Spilkovo - http://acm.timus.ru/problem.aspx?num=2004
 *
 * Strategy:
 * It turns out that the binary numbers we need to encode are the numbers between 0 to 2^n-1. We
 * then brute-force generate the requested string by adding one bit at a time to the bit string and
 * backtracking whenever the n last bits have already been added previously. Although, when running
 * the program I noticed it never actually backtracks, so we won't even have to do that.
 *
 * Performance:
 * O(2^n), runs the test suite in 0.015s using 2248KB memory.
 */

#include <stdio.h>
 
bool A[(1<<20)]; // A[i] = did we check number i yet?
char seq[(1<<20)+20]; // The output
 
int main()
{
    int n;
    scanf("%d", &n);
    int w = (1 << n), p = n, W = w + n - 1;
    for(int i = 0; i < n; i++)
        seq[i] = '1';
    for(int m = ((w - 1) << 1) & (w-1); p < W; m = (m << 1) & (w-1), p++)
        if(A[m])
            seq[p] = '1', A[m |= 1] = true;
        else
            seq[p] = '0', A[m] = true;
    printf("%s", seq);
}