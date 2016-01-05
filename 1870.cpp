/* 1870. Zinium 2 - http://acm.timus.ru/problem.aspx?num=1870
 *
 * Strategy:
 * The commented brute-force code was run and a pattern was found which the below program generates.
 *
 * Performance:
 * O(N), runs the tests in 0.015s using 200KB memory.
 */

#include <stdio.h>

int main()
{
    int N;
    scanf("%d", &N);
    if(N % 6 != 1 && N % 6 != 5)
        printf("No\n");
    else
    {
        printf("Yes\n");
        for(int k = 1; k <= 2; k++)
            for(int i = 0; i*2+k <= N; i++)
                printf("%d ", i*2+k);
    }
}


/*
#include <stdio.h>

int A[100][100], ans[100], N;


bool cross(int i, int j, int n)
{
    if(A[i][j])
        return false;
    for(int c = 0; c < N; c++)
    {
        A[(i+c)%N][(j+c)%N] += n;
        A[(i+c)%N][(N+j-c)%N] += n;
        A[(i+c)%N][j] += n;
        A[i][(j+c)%N] += n;
    }
    return true;
}

void clear(int i, int j, int n)
{
    for(int c = 0; c < N; c++)
    {
        A[(i+c)%N][(j+c)%N] -= n;
        A[(i+c)%N][(N+j-c)%N] -= n;
        A[(i+c)%N][j] -= n;
        A[i][(j+c)%N] -= n;
    }
}

bool dfs(int n)
{
    if(n > N)
        return true;
    //for(int l = (n-1 <= N/2 ? 0 : 1); l < N; l += 2)
    for(int l = 0; l < N; l++)
    {
        if(!cross(n-1, l, n))
            continue;
        else if(dfs(n+1))
        {
            ans[n-1] = l;
            return true;
        }
        else
            clear(n-1, l, n);
    }
    return false;
}

int main()
{
    for(N = 1; ; N++)
    {
        for(int k = 0; k <= N; k++)
            for(int l = 0; l <= N; l++)
                A[k][l] = 0;
        printf("%d: ", N);
        if(dfs(1))
            for(int j = 0; j < N; j++)
                printf("%d ", ans[j]);
        printf("\n");
    }
}*/