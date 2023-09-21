/* 2164. Bookmaker - http://acm.timus.ru/problem.aspx?num=2164
 *
 * Strategy:
 * Brute-force playout of every size-63 substring, where we represent each player's board position
 * as a bitmask and where we precalculate and map each board coordinate to all the bitmasks
 * representing the winning 4-in-a-row chips it helps form. For any given move we then check if
 * the resulting board position contains any the winning 4-in-a-rows for the coordinate of that
 * move.
 *
 * Performance:
 * O(N), runs the tests in 0.015s using 172KB memory.
 */

#include <stdio.h>
#include <vector>

const int maxn = 10000;
int N;
using ll = long long;
char S[maxn+1]; // The input

ll A[2]; // Board positions

std::vector<ll> W[64]; // Winning positions

ll p(int x, int y) { // Turns a position into its bit in the bitmask
    return 1ll << (y*9+x);
}

int solve(int s) {
    int h[9] = { 0 }; // The height of each column
    A[0] = A[1] = 0; // Initialize the positions
    int b = 0; // The player

    for(int i = s; i < std::min(s+63, N); i++, b^=1) {
        int x = S[i] - '1', y = h[x];
        if(y < 7) {
            A[b] |= p(x, y); // Add the coordinate to the player's position
            for(auto w : W[y*9+x]) // For each winning position
                if((A[b] & w) == w) // Check if the board contains it
                    return 1; // If so, a player wins
            if((A[0] | A[1]) == (~(1ll << 63))) // Board is full
                return 2;
            h[x]++;
        }
    }
    return 0;
}

int main() {
    // Precalculate all the winning positions
    for(int y = 0; y < 7; y++) { // Horizontals
        for(int x = 0; x < 6; x++) {
            ll w = 0;
            for(int dx = 0; dx < 4; dx++)
                w |= p(x+dx, y);
            for(int dx = 0; dx < 4; dx++) {
                W[y*9+(x+dx)].push_back(w);
            }
        }
    }
    for(int x = 0; x < 9; x++) { // Verticals
        for(int y = 0; y < 4; y++) {
            ll w = 0;
            for(int dy = 0; dy < 4; dy++)
                w |= p(x, y+dy);
            for(int dy = 0; dy < 4; dy++) {
                W[(y+dy)*9+x].push_back(w);
            }
        }
    }
    for(int x = 0; x < 6; x++) { // Diagonals
        for(int y = 0; y < 4; y++) {
            ll w = 0, w2 = 0;
            for(int dt = 0; dt < 4; dt++) {
                w |= p(x+dt, y+dt);
                w2 |= p(x+3-dt, y+dt);
            }
            for(int dt = 0; dt < 4; dt++) {
                W[(y+dt)*9+x+dt].push_back(w);
                W[(y+dt)*9+x+3-dt].push_back(w2);
            }
        }
    }
    scanf("%s%n", S, &N);

    int O[3] = { 0 };
    for(int i = 0; i < N; i++)
        O[solve(i)]++;
    printf("%d %d", O[1], O[2]);
}
