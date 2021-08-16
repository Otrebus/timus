/* 1311. Stable Construction - http://acm.timus.ru/problem.aspx?num=1311
 *
 * Strategy:
 * For each row, top to bottom, and for each brick, use binary search to find the supporting brick
 * and check that the center of mass is within the endpoints of the supporting brick, then update
 * the latter brick's mass and center of mass.
 *
 * Performance:
 * O(Nlog N), where N is the total number of bricks, runs the test cases in 0.078s using 3,552KB
 * memory.
 */

#include <stdio.h>
#include <vector>

using ld = long double;
const ld eps = 1e-6;

struct brick {
    int L, R, w; // Left and right endpoints, weight
    ld p; // Center of mass
};

std::vector<brick> bricks[10000];

int main() {
    int H, K, L, R;
    scanf("%d", &H);
    for(int i = 0; i < H; i++) {
        scanf("%d", &K);
        for(int j = 0; j < K; j++) {
            scanf("%d %d", &L, &R);
            bricks[i].push_back({ L, R, R-L, ld(R+L)/2 });
        }
    }
    for(int i = 0; i < H-1; i++) { // For each row
        for(auto& b : bricks[i]) { // For each brick
            auto st = bricks[i+1].rbegin(), en = bricks[i+1].rend();
            // Find the brick under us
            auto it = std::lower_bound(st, en, b.R, [] (const brick& a, int R) { return a.L > R; });
            if(it == en || b.p > it->R - eps || b.p < it->L + eps)
                return printf("No"), 0; // No bricks in the row, or center of mass outside brick
            it->p = (it->w*it->p + b.w*b.p)/(it->w + b.w); // Update center of mass of the support
            it->w = it->w + b.w; // Update weight
        }
    }
    printf("Yes");
}
