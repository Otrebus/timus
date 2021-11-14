/* 1397. Points Game - http://acm.timus.ru/problem.aspx?num=1397
 *
 * Strategy:
 * Sort the points by the sum of distances to the other points, and for each player, always pick
 * the largest-sum point. The player who goes first always wins (because if the second player did,
 * the first player could instead play the first move of player two without the penalty of being
 * second to move, guaranteeing him the win, a contradiction).
 *
 * Performance:
 * O(nlog n), runs the tests in 0.015s using 188KB memory.
 */

#include <stdio.h>
#include <algorithm>
#include <cmath>
#include <cstring>

struct point {
    double x, y, d; // Coordinates, sum of distances to other points
    point operator- (point& p) { return { x-p.x, y-p.y }; }
    double l() { return std::sqrt(x*x+y*y); }
} P[1000];

int main() {
    int n;
    while(scanf("%d", &n) != EOF) {
        n*= 2;
        double S = 0;
        std::memset(P, 0, sizeof(P));

        for(int i = 0; i < n; i++)
            scanf("%lf %lf", &P[i].x, &P[i].y);

        // Calculate the distance sum for each point
        for(int i = 0; i < n; i++)
            for(int j = 0; j < n; j++)
                P[i].d += (P[i]-P[j]).l();

        // Sort by distances sum
        std::sort(P, P+n, [] (point& a, point& b) { return a.d > b.d; });

        // The score player one gains from his plays
        for(int i = 0; i < n; i+=2) 
            for(int j = i+2; j < n; j+=2)
                S += (P[i]-P[j]).l();

        // The score difference the second player subtracts from his plays
        for(int i = 1; i < n; i+=2) 
            for(int j = i+2; j < n; j+=2)
                S -= (P[i]-P[j]).l();

        printf("%.3lf\n", S);
    }
}
