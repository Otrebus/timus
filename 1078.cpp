/* 1078. Segments - http://acm.timus.ru/problem.aspx?num=1078
 *
 * Strategy:
 * For each segment, hold a variable that keeps track of the maximum depth of any chain of segments
 * it contains along with a link to its "parent" segment in this chain. Sort the segments by size,
 * and for each segment, make its "parent" the segment that it fits into that has the greatest 
 * depth (and update the total depth). Output the succession line with the largest depth.
 * 
 *
 * Performance:
 * O(N^2), running the tests in 0.015s using 312KB memory. This is not optimal since solutions 
 * running in O(N log N) exist; this latter limit is optimal (theoretically, for numbers of 
 * arbitrary width) since this problem can be reduced to the sorting problem.
 */

#include <vector>
#include <stdio.h>
#include <algorithm>

struct segment
{
    int x, y, n, depth;
    segment* parent;
    segment(int x, int y, int n, segment* parent) : x(x), y(y), n(n), parent(parent), depth(1) { }
};

int main()
{
    int N, x, y;
    std::vector<segment> v;
    scanf("%d", &N);
    for(int i = 1; i <= N; i++)
    {
        scanf("%d %d", &x, &y);
        v.push_back(segment(x, y, i, 0));
    }
    std::sort(v.begin(), v.end(), [] (const segment& a, const segment& b) 
                                         { return a.y-a.x > b.y-b.x; });
    int max = 1;
    segment* maxp = &v[0];
    for(int i = 0; i < N; i++)
    {
        int maxd = 0;
        for(int j = 0; j < i; j++)
        {
            if(v[i].x > v[j].x && v[i].y < v[j].y)
            {
                if(v[j].depth + 1 > maxd)
                {
                    v[i].depth = maxd = v[j].depth + 1;
                    v[i].parent = &v[j];
                    if(maxd > max)
                    {
                        max = maxd;
                        maxp = &v[i];
                    }
                }
            }
        }
    }

    printf("%d\n", max);
    for(segment* e = maxp; e; e = e->parent)
        printf("%d ", e->n);
    return 0;
}