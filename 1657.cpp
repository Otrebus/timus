/* 1657. Cube Snake - http://acm.timus.ru/problem.aspx?num=1657
 *
 * Strategy:
 * Brute-force DFS.
 *
 * Performance:
 * Runs the tests in 0.031s using 184KB memory.
 */

#include <array>
#include <stdio.h>

// Some of these arrays are offset to allow for negative indexing
char s[28], ans[27]; // Input, output
std::array<int, 4> D_[20]; // Valid directions to rotate moving in this direction
auto* D = D_ + 10;
bool V_[20][27]; // Valid to move from position x to y?
bool (*V)[27] = (bool (*)[27])((bool *)&V_ + 10*27);
char C_[256]; // Maps direction to letter
char* C = C_ + 128;

bool dfs(int M, int i, int d, int p) {
    // M is the bitfield of the positions visited, i is the current position,
    // d is the current depth, p is the previous position
    if(M & (1 << p)) // Already visited
        return false;
    ans[i-1] = C[d]; // Record the answer
    if(i == 26) // Finished!
        return true;

    M |= (1 << p);
    if(s[i] == 'T') { // If this is a turning point
        for(auto dp : D[d]) // Attempt rotation in every valid dir given the incoming dir
            if(V[dp][p] && dfs(M, i+1, dp, p + dp)) // If that dir isn't in empty space, recurse
                return true;
        return false;
    }
    return V[d][p] && dfs(M, i+1, d, p + d); // If not a turning point, keep going if inside cube
}

int main() {
    D[1]=D[-1] = { -3, 3, 9, -9 }, D[3]=D[-3] = { -1, 1, 9, -9 }, D[9]=D[-9] = { -1, 1, -3, 3 };
    C[-1] = 'L'; C[1] = 'R'; C[3] = 'F'; C[-3] = 'B'; C[9] = 'U'; C[-9] = 'D';

    for(int p = 0; p < 27; p++)
        for(auto d : { -1, 1, -3, 3, -9, 9 })
            // Valid moves preserve the position modulo 1, 3 or 9 depending on direction
            V[d][p] = (27+p+d)/(d*3) == (27+p)/(d*3);

    scanf("%s", s);
    for(int p : { 0, 3, 10, 12 }) // Because of symmetry we only need to start searching from these
        if(dfs((1 << p), 1, 1, p+1))
            return printf("%s", ans), 0;
    printf("IMPOSSIBLE");
}
