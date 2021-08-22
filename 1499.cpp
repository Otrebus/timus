/* 1499. Kerchiefs - http://acm.timus.ru/problem.aspx?num=1499
 *
 * Strategy:
 * Iterate around the vertices of the polygon. If there are cuts at the current vertex, pick the cut
 * with the narrowest angle (cutting away the largest area), follow it to the opposite vertex and
 * continue the path like above until reaching the original vertex. Cut the convex area that our
 * path formed like a triangle fan (from one vertex to the others), and recurse the algorithm into
 * the areas that we cut away.
 *
 * Performance:
 * O(N log N), runs the tests in 0.046s using 1,936KB memory.
 */

#include <stdio.h>
#include <vector>
#include <algorithm>

struct pair { int a, b; };
std::vector<int> v[50000]; // The cuts
pair ans[50000]; int an; // The output
pair s[50000]; int sn; // The areas we cut away to be recursed into
int w[50000]; // The current area vertices

int main() {
    int N, K, a, b;

    scanf("%d %d", &N, &K);
    while(K--) {
        scanf("%d %d", &a, &b);
        v[std::min(a,b)-1].push_back(std::max(a,b)-1);
    }
    for(int i = 0; i < N; i++) // Sort the cuts by angle formed with the previous vertex
        std::sort(v[i].begin(), v[i].end(), [&i,&N] (int x, int y) { return (x-i+N)%N<(y-i+N)%N; });

    s[sn++] = {0, N-1}; // The areas that we cut away described by start and end vertices
    for(int k = 0; sn; k = 0) {
        auto p = s[--sn]; // Pick some area
        w[k++] = p.a; // Initialize the convex area with the first vertex
        for(int i = p.a; i != p.b; w[k++] = i) { // Add a vertex at each iteration
            if(v[i].empty()) // No cut, just continue along the perimeter
                i++;
            else {
                auto j = v[i].back(); // Remove the cut so we don't follow it again
                v[i].pop_back();
                s[sn++] = {i, j}; // Remember to recurse into the cutaway
                i = j; // Continue our path at the opposite side
            }
        }
        for(int j = 2; j < k-1; j++) // Triangulate the vertices we added
            ans[an++] = { p.a, w[j] };
    }
    printf("%d\n", an);
    for(int i = 0; i < an; i++)
        printf("%d %d\n", ans[i].a+1, ans[i].b+1);
}
