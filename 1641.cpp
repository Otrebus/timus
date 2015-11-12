/* 1641. Duties - http://acm.timus.ru/problem.aspx?num=1641
 *
 * Strategy:
 * Greedish brute-force kind of thing, first match up pairs among only unmatched pairs, then 
 * match up pairs in which ever other way is still possible.
 *
 * Performance:
 * O(mn^2), runs the test cases in 0.015s using 212KB memory.
 */

#include <stdio.h>

const int max = 101;

bool c[101][101];
int p[101];
int room[101];

bool findmatch(int n)
{
    for(int i = 1; i <= n; i++)
    {
        if(!p[i])
        {
            for(int j = 1; j <= n; j++)
            {
                if((!p[j] || (!p[i] && i == n)) && room[i] != room[j] && !c[i][j])
                {
                    p[i]++; 
                    p[j]++;
                    c[i][j] = c[j][i] = true;
                    printf("%d %d\n", i, j);
                    return true;
                }
            }
        }
    }
    return false;
}

int main()
{
    int n, k, m;
    scanf("%d %d %d", &n, &k, &m);
    for(int i = 0; i < n; i++)
        printf("%d\n", room[i+1] = 1 + i % k);

    while(m > 0)
    {
        if(findmatch(n))
            m--;
        else
        {
            bool done = false;
            for(int i = 1; i <= n && !done; i++)
            {
                for(int j = i+1; j <= n && !done; j++)
                {
                    if(!c[i][j] && room[i] != room[j] && i != j)
                    {
                        printf("%d %d\n", i, j);
                        if(!--m)
                            done = true;
                    }
                        
                }
            }
        }
    }
    return 0;
}