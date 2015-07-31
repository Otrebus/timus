/* 1090. In the Army Now - http://acm.timus.ru/problem.aspx?num=1090
 *
 * Strategy:
 * For each line, use a Fenwick tree initialized to zero and invert all input from highest to 
 * lowest (N+1-i for each input i). When reading each line k, for every number i of that line, 
 * use the prefix sum operation of the tree from 1 to i to determine how many numbers are smaller 
 * (originally greater/taller) than the current number, and then add 1 to index i of the tree to 
 * account for the current number before moving on to the next.
 *
 * Performance:
 * O(knlog n), which runs at 0.031s (significantly faster than almost all other solutions) and 
 * uses 968KB of memory.
 */

#include <stdio.h>
#include <algorithm>
#include <vector>

int A[20][10001] = {0}; // Fenwick trees preinitialized
int n;

int sum(int k, int i) // Prefix sum from 1 to 1 of tree k
{
    int sum = 0;
    while(i > 0) 
    {
        sum += A[k][i];
        i -= i & -i;
    }
    return sum;
}
 
void inc(int k, int i)
{
    while (i <= n) 
    {
        A[k][i] += 1;
        i += i & -i;
    }
}

int main()
{
    int max = 0, maxi = 0;
    int k;
    scanf("%d %d", &n, &k);
    for(int i = 0; i < k; i++)
    {
        int num = 0;
        for(int j = 0; j < n; j++)
        {
            int x;
            scanf("%d", &x);
            num += sum(i, n+1-x);
            inc(i, n+1-x);
        }
        if(num > max)
        {
            max = num;
            maxi = i;
        }
    }
    printf("%d", maxi+1);
    return 0;
}