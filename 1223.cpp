/* 1223. Chernobyl Eagle on a Roof - http://acm.timus.ru/problem.aspx?num=1223
 *
 * Strategy:
 * Ideally, given enough eggs, we would do a simple binary search to find E and derive our actual 
 * solution from this since we can break all the eggs that we want. However, if the number of eggs 
 * are limited, we might want to have the binary cut be closer to the ground in case we run out of 
 * eggs and have to try one floor at a time with our last egg. We find the actual solution by 
 * dynamic programming: If we have calculated the solutions A[s, t] for all s < e and all t < v,
 * where e is the number of eggs and v is the number of floors, we can try a solution for s = e 
 * and t = v by considering binary cut at floors k, for all 1 <= k <= v. Such a cut means we will 
 * recurse either by breaking an egg and continuing at A[s-1, t-1] or not breaking the egg and 
 * continuing with e eggs on the t-k upper floors with a total number of steps given by A[s, t-k],
 * with the worst case being the maximum of these two cases.
 * The solution for A[s, t] is then the minimum of all these solutions for the different k's, plus
 * one try.
 *
 * To optimize this, we note that given a higher than logarithmic number of eggs to any given floor
 * we know the answer directly (just a binary search with a logarithmic number of tries), and that
 * the trials for 1 <= k <= v yield a a convex pattern of results.
 *
 * Performance:
 * Roughly O(N log N) where N is 1000, which in practice solves the test cases in 0.015s which is
 * faster than all but very few submissions, and uses 2156KB of memory.
 */

#include <stdio.h>
#include <algorithm>

short A[1001][1001];
short lg[1001]; // Home brewn integer log

int main()
{
    lg[0] = lg[1] = 1;
    for(short i = 0; i <= 1000; i++)
    {   // Edge cases
        A[1][i] = i;
        A[i][0] = 0;
    }
    for(short i = 2; i <= 1000; i++)
        lg[i] = 1 + lg[i/2];

    for(short e = 2; e <= 10; e++) // For e's higher than this we can yield the result immediately
    {
        for(short v = std::max(short(2), (short) pow(2, e)); v <= 1000; v++)
        {   
            short min = 1001;
            for(short k = 1; k <= v; k++)
            {  
                // Here we have to be careful since we might not have stored some of the answers
                // that we are looking for, namely the ones where e is greater than the logarithm
                // of the floor:
                short p1 = lg[k-1] <= e-1 ? lg[k-1] : A[e-1][k-1];
                short p2 = lg[v-k] <= e ? lg[v-k] : A[e][v-k];
                short p = std::max(p1, p2);
                if(p < min)
                    min = p;
                else if(p > min) // Break early because of convexity
                    break;
            }
            A[e][v] = 1 + min;
        }
    }
    while(true)
    {
        int e, v;
        scanf("%d %d", &e, &v);
        if(e == 0 && v == 0)
            break;
        printf("%d\n", lg[v] <= e ? lg[v] : A[e][v]);
    }
    return 0;
}