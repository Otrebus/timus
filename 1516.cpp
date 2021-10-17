/* 1516. Nostalgia - http://acm.timus.ru/problem.aspx?num=1516
 *
 * Strategy:
 * Brute-force; for every white piece, move it in every admissible way but also "filching" any
 * piece that prevents its movement, and report the path that removes all black pieces and filches
 * the fewest number of pieces in total.
 *
 * Performance:
 * Runs the tests in 0.015s using 228KB memory.
 */

#include <stdio.h>
#include <algorithm>
#include <vector>

using ll = long long;

char A[8][8]; // Input
int wp[12], bp[12]; // Positions of white and black pieces
int c[1<<12]; // Counts number of bits in a number
ll mb[1<<12]; // Black board of a given draught bitmask

// Directions (intermediary and end positions) reachable from a position
std::vector<std::pair<int, int>> d[64];

ll W, B; // White/black board configurations
int nw, nb; // Number of white and black pieces
int ans = 100;

void dfs(int p, ll W, ll B, int nw, int nb) {
    // Starting at position p, with white board configuration W and black board configuration B
    // with nw and nb number of white and black pieces remaining

    if(!B) // If all black draughts are taken
        ans = std::min(ans, ::nw-nw+::nb-nb);

    for(int i = 0; i < d[p].size(); i++) {
        auto pm = d[p][i].first, p2 = d[p][i].second;

        ll B2 = B, W2 = W;
        int nw2 = nw, nb2 = nb;

        if(B & (1ll << p2)) // Black piece on destination, filch it
            B2 ^= (1ll << p2), nb2--;

        if(W & (1ll << p2)) // White piece on destination, filch it
            W2 ^= (1ll << p2), nw2--;

        if(B & (1ll << pm)) // Black piece in between, remove it and recurse
            dfs(p2, W2, B2 ^ (1ll << pm), nw2, nb2);
    }
}

int main() {
    for(int j = 0; j < 8; j++)
        scanf("%s", A[j]);

    // Calculate the positions of the black/white pieces and the board bitmasks
    for(int i = 0; i < 8; i++)
        for(int j = 0; j < 8; j++)
            if(A[j][i] == 'W')
                wp[nw++] = i*8 + j, W |= (1ll << (i*8 + j));
            else if(A[j][i] == 'B')
                bp[nb++] = i*8 + j, B |= (1ll << (i*8 + j));

    for(int i = 0; i < (1ll << 12); i++) // Count bits
        for(int j = 0; j < 12; j++)
            if(i & (1ll << j))
                c[i]++;
    
    for(int y = 0; y < 8; y++) // Calc every reachable position and its intermediary position
        for(int x = 0; x < 8; x++)
            for(int dx = -1; dx <= 1; dx += 2)
                for(int dy = -1; dy <= 1; dy += 2)
                    if(x+2*dx < 8 && x+2*dx >= 0 && y+2*dy < 8 && y+2*dy >= 0)
                        d[x*8+y].push_back( { (x+dx)*8 + y+dy, (x+2*dx)*8 + y+2*dy } );

    for(int i = 0; i < (1ll << nb); i++) // Calc black board pieces given bitmasks
        for(int j = 0; j < nb; j++)
            if(i & (1 << j))
                mb[i] |= (1ll << bp[j]);

    int min = 64;
    for(int i = 0; i < nw; i++) // For each white start position
        for(int j = 0; j < (1 << nb); j++) // For each black board setup
            dfs(wp[i], W ^ (1ll << wp[i]), mb[j], nw, c[j]);
    
    printf("%d", ans);
}
