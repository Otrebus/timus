/* 1592. Chinese Watches - http://acm.timus.ru/problem.aspx?num=1592
 *
 * Strategy:
 * Sort the watches by time and calculate the amount of time required to wind all watches to
 * 12:00:00. Go through the ordered list of watches, and note that if we calculated the time
 * to wind all watches to the time of some watch k, for the next watch k+1 in the list, all watches
 * have to be wound forward by the difference between k and k+1, except for watch k+1, which
 * does not have to be wound at all. This allows us to recalculate the total winding time for each
 * target time in constant time for each entry in the list.
 *
 * Performance:
 * O(nlog n), runs the test cases in 0.031s using 676KB memory.
 */

#include <stdio.h>
#include <vector>
#include <algorithm>
#include <numeric>

int main()
{
    using uint = unsigned int;
    uint n, h, m, s;
    std::vector<uint> v;
    scanf("%u", &n);
    for(uint i = 0; i < n; i++)
        scanf("%u:%u:%u", &h, &m, &s), v.push_back((s + 60*(m+60*h))%(12*60*60));
    std::sort(v.begin(), v.end());
    uint sum = 12*60*60*n - std::accumulate(v.begin(), v.end(), 0);
    uint min = std::numeric_limits<uint>::max(), mint = 0, prev = 0;
    for(uint i = 0; i < n; prev = v[i++])
        if((sum += n*(v[i]-prev) - 12*60*60) < min)
            mint = v[i], min = sum;
    if(mint < 3600)
        mint += 3600*12;
    printf("%u:%02u:%02u", mint/3600, mint/60%60, mint%60);
}