/* 1523. K-inversions - http://acm.timus.ru/problem.aspx?num=1523
 *
 * Strategy:
 * Maintain a set of k Fenwick trees, where tree number i for each element j keeps track of how 
 * many valid subsequences containing i elements that start with the number j. Iterate from the end 
 * of the input to the beginning. For every input number x, update Fenwick tree number 1 to k
 * accordingly: for each 1<=l<=k, each sequence of length l that starts with a number less than x 
 | becomes part of a sequence of length l+1.
 *
 * Performance:
 * O(nk log n), running the tests in 0.015s using 1056KB memory; this is the fastest rated solution
 * from any of the 884 submitters!
 */

#include <stdio.h>
#include <algorithm>

int A[11][20001];
int input[20001];
int n, k;
const int mod = 1000000000;

inline int sum(int t, int i)
{
    int sum = 0;
    for(;i > 0; i -= i & -i) 
        sum = (sum + A[t][i]) % mod;
    return sum;
}
 
inline void add(int t, int i, int a)
{
    for (;i <= n; i += i & -i) 
        A[t][i] = (A[t][i] + a) % mod;
}

int main()
{
    scanf("%d %d", &n, &k);
    for(int i = 0; i < n; i++)
        scanf("%d", input+i);
    for(int i = n-1; i >= 0; i--)
    {
        int x = input[i];
        add(1, x, 1);
        for(int l = 1; l < k; l++)
            add(l+1, x, sum(l, x-1));
    }
    printf("%d\n", sum(k, n));
    return 0;
}