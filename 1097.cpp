/* 1097. Square Country 2 - http://acm.timus.ru/problem.aspx?num=1097
 *
 * Strategy:
 * Add the x and y coordinates of the farthest horizontal and vertical edge of each input square,
 * along with the edge of the world. For each pair of those x and y coordinates, try placing the
 * park, and report the minimum cost.
 *
 * Performance:
 * O(M^3), runs the tests in 0.015s using 172KB memory.
 */

#include <stdio.h>
#include <algorithm>
#include <vector>

int V[100], W[100], x[100], y[100]; // Costs, widths, coordinates of the squares
int L, A, M;

int calc(int x1, int y1, int x2, int y2, int i) {
    // Checks if the area with corners (x1, y1) and (x2, y2) intersects square i, and returns
    // the cost if they do, and 1 otherwise (and 255 if the area is not within bounds)
    if(x1 + A > L+1 || y1 + A > L+1)
        return 255;
    int xl = x[i], yl = y[i], xu = x[i]+W[i], yu = y[i]+W[i];
    return (yu > y1 && xu > x1 && y2 > yl && x2 > xl) ? V[i] : 1;
}

int main() {
    std::vector<int> vx { 1 }, vy { 1 };
    scanf("%d %d %d", &L, &A, &M);
    for(int i = 0; i < M; i++) {
        scanf("%d %d %d %d", &V[i], &W[i], &x[i], &y[i]);
        vx.push_back(x[i] + W[i]);
        vy.push_back(y[i] + W[i]);
    }

    int min = 255;
    for(auto xl : vx) {
        for(auto yl : vy) {
            int max = 1;
            for(int i = 0; i < M; i++) 
                max = std::max(max, calc(xl, yl, xl+A, yl+A, i));
            min = std::min(min, max);
        }
    }
    if(min == 255)
        return printf("IMPOSSIBLE"), 0;
    printf("%d", min);
}
