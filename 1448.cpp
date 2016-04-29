/* 1448. Lighting in Hogwarts - http://acm.timus.ru/problem.aspx?num=1448
 *
 * Strategy:
 * Greedy. Add 0's or 1's to a sequence so that the number of 0's and 1's added so far is within 1
 * from the ideal ratio. Then, any subsequence (not necessarily starting from the first number)
 * will be within 2 from the ideal ratio, as specified.
 * Proof: Denote the required ratio of 1's by r = b/100, and the number of 1's in the subsequence 
 * between indices 0 to i by n_i. For any arbitrary subsequence, between indices i and j, say,
 * we have | n_i - ri | <= 1 and | n_j - rj | <= 1, from our greedy strategy. By the triangle 
 * inequality this means that | (n_j - n_i) - r(j - i) | = | n_j - rj - (n_i - ri) | <=
 * | n_j - rj | + | n_i - ri | <= 1 + 1, which is the problem requirement.
 *
 * Performance:
 * The complexity is O(N) and the program runs the test suite in 0.031s using 164KB memory.
 */

#include <stdio.h>

int main()
{
    int N, b;
    scanf("%d %d", &N, &b);
    for(int i = 0, n = 0; i < N; i++)
        if(i*b >= 100*n)
            n++, putchar('1');
        else
            putchar('0');
}