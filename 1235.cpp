/* 1235. Cricket Field - http://acm.timus.ru/problem.aspx?num=1235
 *
 * Strategy:
 * For each pair of points and the horizontal plane segment between them, the maximum vertical span
 * that does not contain any other point is a rectangle that contains the candidate for the answer.
 *
 * Performance:
 * O(N^3 log N), runs the tests in 0.015s using 284KB memory.
 */

#include <iostream>
#include <vector>
#include <algorithm>

struct point { int x, y; } points[102];

int main()
{
    int n, w, h;
    std::cin >> n >> w >> h;
    for(int i = 0; i < n; i++)
        std::cin >> points[i].x >> points[i].y;
    points[n++] = { 0, 0 }; // Sentinel points for the edges
    points[n++] = { w, h };
    point maxp;
    int maxw = 0;
    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < n; j++)
        {   // For each pair of points ..
            if(i == j || points[i].x > points[j].x)
                continue;
            int l = points[i].x, r = points[j].x, max = 0, maxi = 1;
            std::vector<int> v( { 0, h } );
            // .. check the other points and see if they fall within this horizontal segment
            for(int k = 0; k < n; k++)
                if(points[k].x > l && points[k].x < r)
                    v.push_back(points[k].y);
            // Sort these points vertically to find the maximum span with no points
            std::sort(v.begin(), v.end());
            for(int i = 1; i < v.size(); i++)
                if(v[i] - v[i-1] > max)
                    max = v[i] - v[i-1], maxi = i;
            int side = std::min(v[maxi] - v[maxi-1], r-l); // We need a square, not a rectangle
            if(side > maxw)
            {
                maxw = side;
                maxp = { l, v[maxi-1] };
            }
        }
    }
    std::cout << maxp.x << " " << maxp.y << " " << maxw;
}