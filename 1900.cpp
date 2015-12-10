/* 1900. Brainwashing Device - http://acm.timus.ru/problem.aspx?num=1900
 *
 * Strategy:
 * Dynamic programming. Calculate the subsolutions for the case where the device was used k times
 * prior to span i and use these to calculate the optimal subsolutions for k+1 for each span j > i.
 *
 * Performance:
 * O(n^3), runs the tests in 0.327s using 5132KB memory.
 */

#include <stdio.h>
#include <cctype>
#include <algorithm>
#include <stack>
#include <vector>

inline int getint()
{
    int ret = 0, ch;
    while(!isdigit(ch = getc(stdin)));
    ret = ch - '0';
    while(isdigit(ch = getc(stdin)))
        ret = ret*10 + ch - '0';
    return ret;
}

int B[500][500]; // E[i] are the values (number of people) riding the spans starting at i
int E[500][500]; // Same as above but for the spans ending at i
int A[500][500]; // A[i][j] is the sum of all values of all spans that start on or after i that
                 // run through j
int S[500][500]; // S[i][k] is the solution for span numbers < i, having picked k spans
int P[500][500]; // P[i][k] is the previous cutting point of 
                 // the optimal solution at this cutting point

int main()
{
    int n = getint()-1, K = getint();
    
    // Prepare the lists of endpoints and start points
    for(int i = 0; i < n; i++)
    {
        for(int j = i; j < n; j++)
        {
            int v = getint();
            B[i][j-i] = v;
            E[j][i] = v;
        }
    }

    // Calculate the result of cutting through at points j on the subset 
    // of the input that was previous cut at points i
    for(int i = 0; i < n; i++)
    {
        int c = 0;
        for(int j = i; j < n; j++)
        {
            for(int k = 0; k < n-j; k++)
                c += B[j][k];
            A[i][j] = c;
            for(int k = i; k <= j; k++)
                c -= E[j][k];
        }
    }

    // The actual DP step as given in the header comment
    for(int k = 0; k < K; k++)
        for(int i = 0; i < n; i++)
            for(int j = i; j < n; j++)
                if(S[i][k] + A[i][j] >= S[j+1][k+1])
                    S[j+1][k+1] = S[i][k] + A[i][j], P[j+1][k+1] = i;

    // Print out the solution
    int max = 0, j = 0;
    for(int i = 0; i <= n; i++)
        if(S[i][K] >= max)
            j = i, max = S[i][K];
    printf("%d\n", max);
    std::stack<int> s;
    for(int k = K; k; k--)
    {
        s.push(j);
        j = P[j][k];    
    }
    while(!s.empty())
        printf("%d ", s.top()), s.pop();
    
    return 0;
}