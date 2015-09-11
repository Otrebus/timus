/* 1651. Shortest Subchain - http://acm.timus.ru/problem.aspx?num=1651
 *
 * Strategy:
 * Go through each given vertex in turn, updating its previous vertex in terms of its index if
 * a shorter path was found to the current number. This means that whenever a new shortest path
 * was found to some number, a new chain of shortest numbers is started that, if terminating at
 * the final number, becomes the solution. This is possible since a new chain will never be able
 * to modify the shortest path of an earlier chain of numbers because of the ordering constraint.
 *
 * Performance:
 * Linear, runs in 0.031s using 1096KB memory (3rd best rated out of 1194).
 */

#include <stdio.h>

// Distance to this number, previous number in optimal chain, input, output and
// previous index position of an optimal solution path for each number
int d[10001], pre[100001], input[100001], output[10001], pos[10001];

int main()
{
    int N, x, prev = 0, a = 0;
    scanf("%d", &N);
    for(int i = 1; i <= N; i++)
    {
        scanf("%d", &x);
        input[i] = x;
        if(!d[x] || d[prev] + 1 <= d[x])
        {
            pre[i] = i - 1;
            d[x] = d[prev] + 1;
            pos[x] = i;
        }
        else
            pre[i] = pre[pos[x]];
        prev = x;
    }
    for(int i = N; input[i] != 0; i = pre[i])
        output[a++] = input[i];
    for(int i = a-1; i >= 0; i--)
        printf("%d ", output[i]);
}