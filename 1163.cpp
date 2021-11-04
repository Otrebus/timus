/* 1163. Chapaev - http://acm.timus.ru/problem.aspx?num=1163
 *
 * Strategy:
 * For any given draught, consider moves that exactly touch, or barely miss, other draughts.
 * Precalculate the result of every such move (in terms of draughts hit), and use boolean minimax.
 *
 * Performance:
 * Something like O((n/2)!n!), where n is the number of draughts, runs the tests in 0.031s using
 * 648KB memory.
 */

#include <stdio.h>
#include <array>
#include <cmath>

using ld = long double;
ld R = 0.4L;

struct vec { // Standard 2d vector stuff
    ld x, y;
    vec operator+ (const vec& a) { return { x+a.x, y+a.y }; }
    vec operator- (const vec& a) { return { x-a.x, y-a.y }; }
    vec operator/ (ld a) { return { x/a, y/a }; }
    vec operator* (ld a) { return { x*a, y*a }; }
    ld operator* (const vec& a) { return x*a.x + y*a.y; }
    ld d2() { return x*x + y*y; };
    ld d() { return std::sqrt(x*x + y*y); };
    vec r() { return { -y, x }; } // Perpendicular vector
    vec n() { return (vec { x, y })/std::sqrt(x*x + y*y); } // Normalized
};

ld eps = 1e-8;

vec P[16]; // The bitmask of the remaining pieces of the board
int H[16][16][4]; // H[i][j][k] = the the result of moving piece i at piece j using method k
int A[2][1<<16]; // The minimax memoization array

bool hits(vec u, vec v) { // Does the halfline along vector u come within 2R units of position v?
    ld f = (v*(u*v/(v.d2())) - u).d();
    return u.n()*v.n() >= 0 && f < 2*R + eps;
}

ld calcM(int i, int j, ld eps, int b) {
    // Calculates the pieces hit by moving draught i towards draught j on the left or right
    // tangent offset by eps
    int M = 1 << i;

    vec v = P[i], c = P[j];
    ld r = 2*R + eps;

    // Calculate the position of the tangent using some geometry + pythagorean theorem
    vec e1 = (c-v).n(), e2 = ((c-v).r()*b).n(); // "Unit vectors" of the formed triangle
    ld d = (c-v).d();
    auto w = (e1*std::sqrt(d*d-r*r)/d + e2*r/d)*std::sqrt(d*d-r*r); // Use pythagorean to get side

    for(int k = 0; k < 16; k++) // Build the hitmask
        if(hits(P[k]-v, w))
            M |= (1 << k);
    return M;
}

bool dfs(int p, unsigned int M) { // If player p wins playing first with board setup M (minimax)
    if(A[p][M] != -1)
        return A[p][M];

    int m = 0xFF & (M >> (8*p));
    if(!m) // We have draughts left, we lose
        return false;

    for(int i = p*8; i < p*8+8; i++) // For any of our pieces
        if(M & (1 << i)) // For any pieces still alive
            for(int j = 0; j < 16; j++) // For any target pieces
                if(i != j)
                    for(int k = 0; k < 4; k++) // For any of the four ways to hit/barely hit it
                        if(H[i][j][k] && !dfs(!p, M & ~H[i][j][k])) // Fire at it
                            return A[p][M] = true; // If the opponent loses, we win

    return A[p][M] = false;
}

int main() {
    std::memset(A, 0xff, sizeof(A));

    for(int i = 0; i < 16; i++)
        scanf("%Lf %Lf", &P[i].x, &P[i].y);

    // Precalculate the hitmasks
    for(int i = 0; i < 16; i++) { // For any draught we shoot
        for(int j = 0; j < 16; j++) { // For any draught we target
            if(i != j) { // Can't target ourselves
                int k = 0; // The shooting method
                for(auto e : { 0.0L, 2*eps } ) // Hitting, barely missing
                    for(auto b : { -1, 1 } )   // Left tangent, right tangent
                        H[i][j][k++] = calcM(i, j, e, b); // Calculate the hitmask
            }
        }
    }

    printf(dfs(0, 0xFFFF) ? "RED" : "WHITE");
}
