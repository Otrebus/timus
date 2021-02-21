/* 1578. Mammoth Hunt - http://acm.timus.ru/problem.aspx?num=1578
 *
 * Strategy:
 * Connect each new point to its farthest unconnected point.
 *
 * Performance:
 * O(N^2), runs the tests in 0.031s using 164KB memory.
 */

#include <stdio.h>

struct point {
    int x, y;
    int operator- (const point& p2) const { return (x-p2.x)*(x-p2.x)+(y-p2.y)*(y-p2.y); }
} P[2001];
int p[2001], a[2001];
int n, x, y, pp = 1, j = 0;

int far(point* pt) {
    int max = 0, maxi = 0;
    for(int i = 1; i <= n + 2; i++)
        if(!p[i] && *pt - P[i] > max)
            max = *pt - P[i], maxi = i;
    return maxi;
}

int main() {
    scanf("%d", &n);
    for(int i = 1; i <= n + 2; i++)
        scanf("%d%d", &P[i].x, &P[i].y);
    while(pp)
        p[pp] = true, a[j++] = pp, pp = far(P + pp);
    printf("YES\n");
    for(int i = 0; i < j; i++)
        printf("%d ", a[i]);
}
