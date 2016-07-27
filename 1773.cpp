/* 1773. Metro to Every Home - http://acm.timus.ru/problem.aspx?num=1773
 *
 * Strategy:
 * Greedy; put each strip in a bucket marked by the coordinate of the left endpoint. Start on the
 * bucket of the maximum coordinate and then jump from bucket to bucket with stride equal to the
 * coordinate difference, picking a strip each time. If a required strip wasn't found in a bucket,
 * try the bucket with the coordinate of the flipped strip.
 *
 * Performance:
 * O(n), runs the tests in 0.046s using 2,568KB memory (rated 4/329).
 */

#include <vector>
#include <stdio.h>
#include <limits>
#include <algorithm>

const int maxn = 50000, maxh = 100000;
std::vector<int> A[maxh+1]; // The buckets described above

int main()
{
    const int oo = std::numeric_limits<int>::max();
    int n, d = oo, h, x, y, max = 0, min = oo;
    auto flip = [&d, &h] (int x) { return d + h - x; };

    scanf("%d %d", &h, &n);
    for(int i = 1; i <= n; i++)
    {
        int x, y;
        scanf("%d %d", &x, &y);
        A[x].push_back(i);
        if(d != oo && x-y != d) // This strip has a different slope
        {
            printf("0");
            return 0;
        }
        d = x-y;
        
        max = std::max( { max, x, flip(x) } );
        min = std::min( { min, x, flip(x) } );
    }

    if(d < 0) // Negative slope, so we are actually interested in the lowest coordinate
        std::swap(max, min);
    std::vector<int> ans;
    for(int i = max; d < 0 ? (i <= min) : (i >= min); i -= d)
    {
        if(!A[i].empty())
            ans.push_back(A[i].back()), A[i].pop_back();
        else if(!A[flip(i)].empty())
            ans.push_back(-A[flip(i)].back()), A[flip(i)].pop_back();
        else
            break;
    }
    if(ans.size() != n)
    {
        printf("0");
        return 0;
    }
    for(auto a : ans)
        printf("%d ", a);
}