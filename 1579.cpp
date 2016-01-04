/* 1579. Coat Transportation - http://acm.timus.ru/problem.aspx?num=1579
 *
 * Strategy:
 * Greedy (and online). For each number x given, we add x + R to a queue to keep track of when we
 * can add to the same set again. Also (actually before that) we check if we can add x to any of
 * the sets in the queue, which is only true if we can add it to the set in the front of the queue
 * since that set has the leftmost earlier added point. If we can't add to any sets in the queue
 * we create a new set.
 *
 * Performance:
 * O(N), executes the test input in 0.561s using 5484KB memory. This can be made faster (and uglier)
 * by faster input/output methods.
 */

#include <vector>
#include <stdio.h>
#include <queue>

std::vector<int> v[100000]; // The sets

int main()
{
    int N, R, m = 0;
    scanf("%d %d", &N, &R);
    std::queue<std::pair<int, int>> q; // The queue, containing { x+R, set number }
    q.push( { -1000000001, 0 } ); // Sentinel point
    for(int i = 1; i <= N; i++)
    {
        int x;
        scanf("%d", &x);
        if(x > q.front().first) // We can add to the front set of the queue
        {
            v[q.front().second].push_back(i); // Add to the set
            q.push( { x+R, q.front().second }); // Move the setinfo to the back
            q.pop();                            // of the queue
        }
        else
        {
            v[q.size()].push_back(i); // We add to a new set
            q.push({ x+R, ++m });     // whose info we push into the queue
        }
    }
    printf("%d\n", m+1);
    std::vector<std::vector<int>> out;
    for(int i = 0; i <= m; i++)
    {
        printf("%d ", v[i].size());
        for(int j = 0; j < v[i].size(); j++)
            printf("%d ", v[i][j]);
        printf("\n");
    }
}