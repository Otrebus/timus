/* 1065. Frontier - http://acm.timus.ru/problem.aspx?num=1065
 *
 * Strategy:
 * Form a graph out of all the pairs of endpoints that can form an edge to the left of any
 * monument (with the edge length as its weight) and solve with all-pairs shortest paths.
 * 
 * Performance:
 * O(N^3 + MN^2), runs the tests in 0.015s using 280KB memory.
 */

#include <stdio.h>
#include <algorithm>
#include <cmath>

using ld = long double;

struct point {
    int x, y;
    point operator- (const point& p) { return { x-p.x, y-p.y }; }
    int operator% (const point& p) { return x*p.y - p.x*y; }
    ld d() { return std::sqrt(ld(x*x+y*y)); }
};

point P[50], E[1000];
ld D[50][50];
int N, M;

int main() {
    scanf("%d %d", &N, &M);
    
    for(int i = 0; i < N; i++)
        scanf("%d %d", &P[i].x, &P[i].y);

    ld min = 1e100;
    if(!M) { // If there are no monuments we check all triplets
        for(int i = 0; i < N; i++)
            for(int j = 0; j < N; j++)
                for(int k = 0; k < N; k++)
                    if((P[i]-P[j])%(P[k]-P[j])) // Not in a straight line
                        min = std::min(min, (P[i]-P[j]).d()+(P[j]-P[k]).d()+(P[k]-P[i]).d());
        return printf("%Lf", min), 0;
    }

    for(int i = 0; i < M; i++)
        scanf("%d %d", &E[i].x, &E[i].y);

    for(int i = 0; i < N; i++)
        for(int j = 0; j < N; j++)
            D[i][j] = D[j][i] = 1e100;
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) { // For any pair of vertices
            bool all = i != j;
            for(int k = 0; all && k < M; k++)
                // If all the monuments are on the right side of these two vertices
                all = all && (E[k]-P[i])%(P[j]-P[i]) > 0;
            if(all)
                D[i][j] = (P[j]-P[i]).d(); // Add them to the graph
        }
    }

    for(int i = 0; i < N; i++) // Floyd-Warshall
        for(int j = 0; j < N; j++)
            for(int k = 0; k < N; k++)
                D[i][j] = std::min(D[i][j], D[i][k] + D[k][j]);

    // Any path from a vertex back to itself must form a non-degenerate figure since we must wrap
    // the path around at least one point; the answer is the minimum length of any such path
    for(int i = 0; i < N; i++)
        min = std::min(min, D[i][i]);
    printf("%Lf", min);
}
