/* 1540. Battle for the Ring - http://acm.timus.ru/problem.aspx?num=1540
 *
 * Strategy:
 * The described game is an impartial game that we can solve using the Sprague-Grundy theorem.
 * Every time we dematerialize rings in a chain, the chain is split into new subgames whose SG
 * values we keep in an array with elements g[k][i][j] which is the sg value for the game which
 * consists of the connected rings between i and j for chain k. We can then loop through all
 * possible splitting positions (while summing the SG values of all chains/subgames together) to
 * return the first one that yields the opponent a position with SG value 0.
 * 
 * Performance:
 * Something like O(KN^3), where N is the maximum size of a chain, runs the tests in 0.062s using
 * 10,700KB memory.
 */

#include <stdio.h>
#include <memory>

int g[50][101][101]; // The SG values
int c[50][101]; // The chains
int l[50], K, G = 0, i, x, y; // Length of the chains, various scrap variables to code golf a little

int sg(int k, int a, int b);

int calcG(int k, int i, int a, int b, int g) {
    // Calculates the SG value for the summed position of the games produced by splitting rings a
    // through b in chain k by removing rings with value less than or equal to the one at position i
    for(g = 0, x = a, y = a; x < b; g ^= sg(k, x, y)) { // Loops over subchains
        for(x = y; x < b && c[k][x] <= c[k][i]; x++); // Skip all smaller values
        for(y = x; y < b && c[k][y] > c[k][i]; y++); // All greater or equal ones become a new chain
    }
    return g;
}

int sg(int k, int a, int b) {
    // Calculates the SG value of chain k between links a and b
    if(g[k][a][b] > -1) // Already memozied
        return g[k][a][b];
    bool gs[102]; // Array used to calculate the mex (minimum excluded value)
    std::memset(gs, 0, 1+b-a);
    for(int i = a; i < b; i++) // Calc the SG values of each follower
        gs[calcG(k, i, a, b, 0)] = true;
    for(i = 0; gs[i]; ++i); // Calculate the mex
    return g[k][a][b] = i; // Return and memoize
}

int main() {
    std::memset(g, 0xff, sizeof(g));
    scanf("%d", &K);
    for(int i = 0; i < K; i++) {
        scanf("%d", &l[i]);
        for(int j = 0; j < l[i]; j++)
            scanf("%d", &c[i][j]);
    }

    for(int k = 0; k < K; k++) // Calculate the SG value of the starting position
        G ^= sg(k, 0, l[k]);
    for(int k = 0; k < K; k++) // For each chain
        for(int i = 0; i < l[k]; i++) // For each splitting position (value)
            // "Subtract" the current chain from the SG value and "add" the positions given
            // by splitting chain k by the value at position i, and check if the opponent ends
            // up with a position with SG value 0, in which case he loses
            if(!(G ^ sg(k, 0, l[k]) ^ calcG(k, i, 0, l[k], 0)))
                return printf("G\n%d %d", k+1, i+1), 0;
    printf("S");
}
