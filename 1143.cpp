/* 1143. Electric Path - http://acm.timus.ru/problem.aspx?num=1143
 *
 * Strategy:
 * First, we note that an optimal path is nonintersecting: if a path contains two intersecting
 * segments that form an "X" with the vertices connected to the rest of the path, we can rearrange
 * the "X" into a "||" or "=" (depending on topology) which gives a shorter overall path. Now we can
 * proceed with dynamic programming as follows:
 * We keep track of all subsolutions for the problem subset that start at vertex i and contains the 
 * following l vertices and has the leftmost vertex as an endpoint of the line (we store this in 
 * the array L[i,l]), and also keep track of the subsolutions for the ranges where the rightmost
 * vertex is the endpoint of the line (R[i,l]). We then fill in the subsolution for each L[i,l+1] by
 * either connecting the leftmost vertex of this range of vertices to the leftmost vertex in
 * L[i+1,l], or to the rightmost free vertex in R[i+1, l] (we choose the one with the minimum total
 * length). We fill in the subsolution for the R[i,l+1]'s similarly. The final answer is then the
 * minimum result over the arrays in L and R that contain all vertices.
 *
 * Performance:
 * O(N^2), runs the test cases in 0.015s using 836KB memory.
 */

#include <stdio.h>
#include <tuple>
#include <cmath>
#include <algorithm>
#include <vector>

const int maxn = 200;

struct point { double x, y; } in[maxn];
double L[maxn][maxn], R[maxn][maxn];

double dist(const point& a, const point& b)
{
    return std::sqrt((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y));
}

int main()
{
    int n;
    scanf("%d", &n);
    for(int i = 0; i < n; i++)
    {
        L[0][i] = R[0][i] = 0;
        scanf("%lf %lf", &in[i].x, &in[i].y);
    }
    for(int l = 1; l < n; l++)
    {
        for(int i = 0; i < n; i++)
        {
            R[i][l] = std::min(R[i][l-1]+dist(in[(i+l)%n], in[(i+l-1)%n]), 
                               L[i][l-1]+dist(in[i], in[(i+l)%n]));
            L[i][l] = std::min(L[(i+1)%n][l-1]+dist(in[i], in[(i+1)%n]),
                               R[(i+1)%n][l-1]+dist(in[i], in[(i+l)%n]));
        }
    }
    double ans = std::numeric_limits<double>::infinity();
    for(int i = 0; i < n; i++)
        ans = std::min( { ans, L[i][n-1], R[i][n-1] } );
    printf("%.3lf\n", ans);
}