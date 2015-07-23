/* 1031. Railway Tickets - http://acm.timus.ru/problem.aspx?num=1031
 *
 * Strategy:
 * For each station, determine the optimal cost to reach this station by looking at the furthest 
 * station that can reach this station, for each of the three types of tickets - this is kept track
 * of in three queues - and choosing the cheapest one. Then add this optimal cost together with
 * the costs of the various tickets into the queues, for the upcoming stations.
 *
 * Performance:
 * O(n) where n is the amount of stations considered, since we loop over those stations, and push
 * and pop from the queues at most a constant number for each station. The solution runs the tests
 * in 0.015s, which is equivalent to or much faster than most solutions, and uses 524KB of memory.
 */

#include <queue>
#include <vector>
#include <tuple>
#include <limits>
#include <algorithm>

int main()
{
    int C[3], L[3], N, s, t;
    std::queue<std::pair<int, int>> q[3]; // <distance, cost> for each type of ticket
    scanf("%d %d %d %d %d %d %d %d %d", L, L+1, L+2, C, C+1, C+2, &N, &s, &t);
    if(s > t)
        std::swap(s, t);
    std::vector<int> v; // Station distances
    v.reserve(N+1);
    v.push_back(0); // For easier indexing
    v.push_back(0); // Station #1, for convenience
    while(--N)
    {
        int x;
        scanf("%d", &x);
        v.push_back(x);
    }
    // Initialize with the ticket ranges from the first station
    for(int i = 0; i < 3; i++)
        q[i].push(std::make_pair(L[i] + v[s], C[i]));
    int c;
    for(int i = s+1; i <= t; i++)
    {
        int l = v[i];
        // First, pop tickets until we get them just in range (and hence optimal)
        for(int i = 0; i < 3; i++)
            while(!q[i].empty() && q[i].front().first < l)
                q[i].pop();
        // Choose the cheapest one of the three optimal types
        std::vector<int> t(3);
        for(int i = 0; i < 3; i++)
            t[i] = q[i].empty() ? std::numeric_limits<int>::max() : q[i].front().second;
        c = *std::min_element(t.begin(), t.end());
        // Project ticket ranges forward
        for(int i = 0; i < 3; i++)
            q[i].push(std::make_pair(L[i]+l, c+C[i]));
    }
    printf("%d\n", c);
    return 0;
}