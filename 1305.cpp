/* 1305. Convex Hull - http://acm.timus.ru/problem.aspx?num=1305
 *
 * Strategy:
 * Calculate the AABB, and then clip this AABB with the 45-degree rotated bounding box.
 *
 * Performance:
 * O(n), runs the test cases in 0.031s using 176KB memory.
 */

#include <array>
#include <stdio.h>

int main()
{
    int N, x, y, w, z;
    scanf("%d", &N);
    // Extreme coordinates along the x-y axis and the rotated z-w axis (our two bounding boxes)
    std::array<int, 8> m = { 2000, 0, 2000, 0, 2000, 0, 2000, 0 };
    while(N--)
    {
        scanf("%d %d", &x, &y);
        w = x+y, z = 1000-x+y;
        // Update the bounding boxes
        m = { std::min(x, m[0]), std::max(x, m[1]), std::min(y, m[2]), std::max(y, m[3]), 
              std::min(w, m[4]), std::max(w, m[5]), std::min(z, m[6]), std::max(z, m[7]) };
    }
    // Clip the two bounding boxes into an octagon
    std::array<std::pair<int, int>, 8> p = { std::pair<int,int> {m[4]-m[2], m[2]},
        {m[2]+1000-m[6],m[2]}, {m[1],m[1]-1000+m[6]}, {m[1],m[5]-m[1]}, {m[5]-m[3],m[3]},
        {1000-m[7]+m[3],m[3]}, {m[0],m[7]-1000+m[0]}, {m[0],m[4]-m[0]} };
    // Print, making sure to reject duplicate points in case the octagon is degenerate
    printf("%d %d\n", p[0].first, p[0].second);
    for(int i = 1; i < 8; i++)
        if((p[i] != p[i-1]) && (p[i] != p[0]))
            printf("%d %d\n", p[i].first, p[i].second);
}