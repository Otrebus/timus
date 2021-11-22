/* 1599. Winding Number - http://acm.timus.ru/problem.aspx?num=1599
 *
 * Strategy:
 * Subtract the number of times the track crosses the y axis (with us as the origin) in a clockwise
 * fashion with the number of times it does so the other way, and divide by two.
 *
 * Performance:
 * O(mn), runs the tests in 0.203s using 244KB memory.
 */

#include <stdio.h>
#include <algorithm>

using ll = long long;

template<typename T> struct point { T x, y; };
point<ll> T[5001];
point<int> P[5001];

int n, m;

int winding(int& x, int& y) {
    int r = 0;
    for(int i = 0; i <= n; i++)
        T[i].x = P[i].x-x, T[i].y = P[i].y-y;

    for(int i = 0; i < n; i++) {
        point<ll>& a = T[i], &b = T[i+1];

        if(a.x*b.x <= 0) { // Crosses the y axis
            auto x = a.x*b.y - b.x*a.y; // Direction
            if(!x && a.y*b.y <= 0)  // No direction, and we're in the middle
                return printf("EDGE\n");
            if(std::min(a.x, b.x) < 0) // Don't double-count
                r += (x > 0) ? 1 : -1; // If ccw, add, otherwise subtract
        }
    }
    return printf("%d\n", r/2);
}

int main() {
    scanf("%d", &n);
    for(int i = 0; i < n; i++)
        scanf("%d %d", &P[i].x, &P[i].y);
    P[n] = P[0];

    scanf("%d", &m);
    for(int i = 0; i < m; i++) {
        int x, y;
        scanf("%d %d", &x, &y);
        winding(x, y);
    }
}
