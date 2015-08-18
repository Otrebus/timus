/* 1218. Episode N-th: The Jedi Tournament - http://acm.timus.ru/problem.aspx?num=1218
 *
 * Strategy:
 * Build an implicit tournament graph; the vertices with indegree N-1 continuously down to some N-k
 * can not win the tournament while all of the rest can. The one exception is some vertex having an 
 * indegree of 0 - then this vertex is the only possible winner.
 *
 * Performance:
 * O(N^2), runs in 0.001s using 200KB memory.
 */

#include <stdio.h>

char names[201][31];
int  stats[201][3];
int  deg[201]; // Indegree of vertices
int  h[201];   // Index of a vertex with the given indegree, guaranteed to be unique for indegrees 0
               // and contiguous indegrees N-1 up to N-k
bool output[201]; // False indicates possible winners

int main()
{
    int N;
    scanf("%d", &N);
    for(int i = 1; i <= N; i++)
        scanf("%s %d %d %d", names[i], stats[i], stats[i]+1, stats[i]+2);
    for(int i = 1; i <= N; i++)
        for(int j = i+1; j <= N; j++)
            if(int(stats[i][0] > stats[j][0]) + int(stats[i][1] > stats[j][1]) 
               + int(stats[i][2] > stats[j][2]) > 1)
                deg[j]++; // Jedi beaten in more than one stat gets one more dominator and hence
            else          // one more indegree
                deg[i]++;
    for(int i = 1; i <= N; i++)
        h[deg[i]] = i;
    if(h[0]) // Special case, if there is a node with no indegrees, this is the only possible winner
        printf("%s\n", names[h[0]]);
    else
    {
        for(int i = N-1; h[i]; i--)
            output[h[i]] = true;
        for(int i = 1; i <= N; i++)
            if(!output[i])
                printf("%s\n", names[i]);
    }
    return 0;
}