/* 1655. Somali Pirates - http://acm.timus.ru/problem.aspx?num=1655
 *
 * Strategy:
 * Sort the ships by clockwise azimuth from the initial gun angle. We can then do dynamic
 * programming over A[L][R][C], where
 * 
 *   A = number of degrees turned in total
 *   L = leftmost ship targeted
 *   R = rightmost ship targeted
 *   C = current target (either L or R above)
 * 
 * To calculate A[L][R][C] we check A[L+1][R][K], or A[L][R-1][K] where K is 0 or 1, (meaning,
 * narrow the range by one in either direction, for either initial gun position) and add the
 * extra angle we needed to turn the gun from there. Keep in mind these numbers are essentially
 * done with modular arithmetic since we're dealing with angles (think the face of a clock). To
 * gain extra accuracy, we turn every real number given into an integer by multiplying by 1e3.
 *
 * Performance:
 * O(n^2), runs the tests in 0.046s using 4,112KB memory.
 */

#include <stdio.h>
#include <algorithm>
#include <array>
#include <vector>

using ld = long double;
using ll = long long;
const ll inf = 1e9;
const ld eps = 1e-6;

struct ship {
    int i;
    ll b, d, v;
} ships[501];

struct ans {
    int cost; // Number of degrees turned
    short pa, pb; // Previous optimal answer
};

ans A[501][501][2];
ll a, w, W;
int n;

int dist(int i, int j, bool cw) { // Calculates the angle (cw or ccw) between two ships
    auto d = ships[j].b - ships[i].b;
    return cw ? d + (d < 0 ? 360000 : 0) : dist(j, i, !cw);
}

ll ti(ld x) { // Turns a real number into an integer (multiplied by 1e3)
    return 1000*(x+eps);
}

int main() {
    ld a1, w1;
    scanf("%Lf %Lf %d", &a1, &w1, &n);
    a = ti(a1), w = ti(w1);

    // We pretend the initial gun angle is a ship
    ships[0] = { 0, a, 10000, 0 };
    A[0][0][1].cost = A[0][0][0].cost = 0;

    for(int i = 1; i <= n; i++) { // Read the input, intify it
        ld b, d, v;
        scanf("%Lf %Lf %Lf", &b, &d, &v);
        ships[i].b = ti(b), ships[i].d = ti(d), ships[i].v = ti(v), ships[i].i = i;
    }

    // Sort the ships by clockwise azimuth from initial gun angle
    auto fn = [] (ship& s1, ship& s2) { return (360000+s1.b-a) % 360000 < (360000+s2.b-a)%360000; };
    std::sort(ships, ships + n + 1, fn);

    for(int I = 0; I >= -n; I--) {
        // Our leftmost ship is 0, -1, -2, -3 ...
        int i = (I+n+1)%(n+1);
        for(int J = 1; J <= n; J++) {
            // Our corresponding rightmost ship is ship i+1, up to ship i+n
            int j = (i+J)%(n+1);
            for(int k = 0; k < 2; k++) { // Current gun angle (left- or rightmost ship)
                int minc = inf;
                short pa = -1, pb = -1; // The previous outermost ships of the optimal value
                for(int k2 = 0; k2 < 2; k2++) { // Previous gun angle
                    int a = (i+1)%(n+1), b = (n+1+j-1)%(n+1);
                    // Search the previous dp values by narrowing the gun direction inwards
                    for(auto p : { std::pair<int, int> { a, j }, { i, b } }) {

                        auto a = p.first, b = p.second;
                        if(b - a >= 0 && a != 0) // The sequence must contain 0 (initial gun pos)
                            continue;

                        int c = 0; // The total angle traveled (potential value of A[i][j][k])
                        // Our gun traveled from ship x at the original position, to y that we
                        // extended the range by and then to z at the new position. Deduce what
                        // these ships must have been
                        int x = k2 == 0 ? a : b, y = (a == i ? j : i), z = k == 0 ? i : j;

                        // Calculate the distances between these ships and which way we turned
                        auto d1 = dist(x, y, j!=b), d2 = dist(y, z, k==1);
                        auto d0 = A[a][b][k2].cost;
                        c = d1 + d2 + d0;

                        // Check that the new ship (the one we extended the range by) didn't have
                        // time to reach within a nautical mile
                        if((d0 + d1)*ships[y].v <= 360*w*60*(ships[y].d-1000) && c < minc)
                            pa = a, pb = b, minc = c;
                    }
                }
                A[i][j][k] = { minc, pa, pb };
            }
        }
    }

    int minc = 1e9;
    int minj = -1, mink = -1;
    
    // Find the optimal range and answer (must be of the form A[b][b-1][c])
    for(int j = 0; j <= n; j++) {
        for(int k = 0; k < 2; k++) {
            auto a = A[(j+1)%(n+1)][j][k];
            if(a.cost < minc) {
                minc = a.cost, minj = j, mink = k;
            }
        }
    }

    if(minc == inf)
        return printf("Impossible"), 0;

    // Use our "previous" values to find the optimal answer path
    std::vector<int> output;
    int j = minj, i = (j+1)%(n+1), k = mink;
    while(i != 0 || j != 0) {
        auto an = A[i][j][k];
        output.push_back(i == an.pa ? j : i);
        k = (i == an.pa), i = an.pa, j = an.pb;
    }

    // Output the answer
    printf("%Lf\n", ld(minc)/(360*w));
    for(auto it = output.rbegin(); it < output.rend(); it++)
        printf("%d\n", ships[*it].i);
}
