/* 1976. Game Optimization - http://acm.timus.ru/problem.aspx?num=1976
 *
 * Strategy:
 * For each cell we maintain the array P[x][y] which is the distance to the closest base above the
 * cell. For each row these bases form a (possibly very much concave) polygon. We maintain the
 * vertices of that polygon in a stack, and iterate the row from left to right and keep calculating
 * where on the row (we call the position ix) a new closest base becomes the closest vertex compared
 * to the previous point on the stack. If this point ix is to the left of the previous calculated
 * such point on the stack, we keep popping the stack if this is no longer true, until we finally
 * add ix (and the x-position of the base we calculated it for) onto the stack. We then iterate
 * through the stack from bottom to top and use the ixes and xes on the stack to quickly find the
 * distance to each closest vertex (base) on the row. This calculates the correct distances upwards
 * and to the left, so we rotate four times and repeat the procedure.
 *
 * Performance:
 * O(nm), runs the tests in 0.468s using 13,848KB memory.
 */

#include <stdio.h>
#include <cstring>
#include <algorithm>

bool b[1000][1000], b2[1000][1000]; // The bases
int a[1000][1000], a2[1000][1000]; // Best distances

int (*A)[1000] = a, (*A2)[1000] = a2;
bool (*B)[1000] = b, (*B2)[1000] = b2;

int P[1000][1000]; // Closest base in the y direction
struct intersect { int ix, x; } S[2000]; // The stack of closest points to polygon vertices
int n, m, s;

void update() {
    for(int y = 0; y < n; y++)
        for(int x = 0; x < m; x++)
            P[x][y] = (B[x][y]) ? y : !y ? -1 : P[x][y-1];

    for(int y = 0; y < n; y++) {
        s = 0;
        for(int x = 0; x < m; x++) {
            if(P[x][y] < 0) continue;
            while(s) {
                // Calculate the point on the row where the new base becomes the closest compared
                // to the top of the stack
                auto p1x = x, p1y = P[x][y], p2x = S[s-1].x, p2y = P[S[s-1].x][y];
                auto A = p1x*p1x + y*y - 2*y*p1y + p1y*p1y, B = p2x*p2x + y*y - 2*y*p2y + p2y*p2y;
                if(2*(p1x-p2x)*S[s-1].ix > A-B) {
                    // If it's to the left of the previous point, pop the stack
                    s--;
                } else { // Otherwise add to the stack
                    S[s++] = { (A-B + 2*(p1x-p2x) - 1)/(2*(p1x-p2x)), x };
                    break;
                }
            }
            S[s++] = { x, x };
        }
        if(s) { // Go through the stack and calculate the closest distances to each base
            for(int j = 0; j < s; j++) {
                auto [ix, x] = S[j];
                for(int i = S[j].ix; i < m && (j == s - 1 || i < S[j+1].ix); i++)
                    A[i][y] = std::min(A[i][y], ((P[x][y]-y)*(P[x][y]-y) + (i-x)*(i-x)));
            }
        }
    }
}

void rotate() { // Rotates the field
    for(int y = 0; y < n; y++) {
        for(int x = 0; x < m; x++) {
            A2[y][m-x-1] = A[x][y];
            B2[y][m-x-1] = B[x][y];
        }
    }
    std::swap(B, B2);
    std::swap(A, A2);
    std::swap(n, m);
    std::memset(P, 0xff, sizeof(P));
}

int main() {
    std::memset(P, 0xff, sizeof(P));
    std::memset(a, 0x3f, sizeof(a));

    scanf("%d %d", &n, &m);
    for(int y = 0; y < n; y++) {
        char s[1001];
        scanf("%s", s);
        for(int x = 0; x < m; x++)
            B[x][y] = s[x]-'0';
    }

    for(int i = 0; i < 4; i++) {
        update();
        rotate();
    }

    for(int y = 0; y < n; y++) {
        for(int x = 0; x < m; x++)
            printf("%d ", A[x][y]);
        printf("\n");
    }
}
