/* 1843. Cutting a Parallelepiped - http://acm.timus.ru/problem.aspx?num=1843
 *
 * Strategy:
 * Start at the bottom front left edge, find the only piece which fits (by trying each piece), then
 * proceed to find edge pieces along its adjoining edges, which we can add in any order. These edge
 * pieces provide the coordinates along the axes for an overall sectioning of the cube, so after
 * having scanned the edges this way, we then have a grid that we can proceed to iterate over to
 * find pieces for the rest of the cube.
 *
 * Performance:
 * O(n^2), runs the tests in 0.046s using 988KB memory.
 */

#include <stdio.h>
#include <vector>
#include <algorithm>

struct piece {
    int D[6]; // The depth behind each side (dimensions of the piece, only three unique numbers)
    char C[6]; // The color of each side
} P[1000][24]; // One for each permutation
bool B[1000]; // Whether we've already added a piece

// Denotes a cube permutation, allows for compositing operations on permutations and permuting a piece
struct permutation {
    int A[6];
    permutation operator* (const permutation& p) {
        permutation result;
        for(int i = 0; i < 6; i++)
            result.A[i] = p.A[A[i]];
        return result;
    }
    piece operator* (const piece& p) {
        piece result;
        for(int i = 0; i < 6; i++)
            result.D[i] = p.D[A[i]];
        for(int i = 0; i < 6; i++)
            result.C[i] = p.C[A[i]];
        return result;
    }
};

struct result { // Denotes a result piece: its index, permutation and position
    int i, p, z, y, x;
};
std::vector<result> res;

// Permutations (rotations around two axes)
permutation rot[4] = {{0,1,2,3,4,5},{5,4,2,3,0,1},{1,0,2,3,5,4},{4,5,2,3,1,0}};
permutation tip[4] = {{0,1,2,3,4,5},{2,3,1,0,4,5},{1,0,3,2,4,5},{3,2,0,1,4,5}};
permutation perms[24];

char C[7]; // The colors of the original cube
int N = 0;
std::vector<int> S[3]; // The width of each section of the cube in each dimension
int c[3] = { 0, 0, 0 }; // The coordinate of the piece that we're currently trying to add

// Tries to match permutation p of piece i with the given colors and dimensions
bool match(int i, int p, char A[6], int D[3], int d) {
    for(int j = 0; j < 6; j++) // Match colors
        if(A[j] != -1 && A[j] != P[i][p].C[j])
            return false;

    for(int j = 0; j < 3; j++) // Match dimensions
        if(D[j] != -1 && D[j] != P[i][p].D[2*j])
            return false;
    
    res.push_back({ i, p, c[0], c[1], c[2] });
    if(d != -1) { // If we're in the edge-scanning stage, update the sectioning info
        S[d].push_back(P[i][p].D[d*2]);
        c[d] += P[i][p].D[d*2];
    }
    return true;
}

// Finds a cube with the given sides (a) and the given widths (b). d denotes if it's part of the
// first edge-scanning stage
bool find(char a1, char a2, char a3, char a4, char a5, char a6, int d1, int d2, int d3, int d) {
    char A[6] = { a1, a2, a3, a4, a5, a6 };
    int D[3] = { d1, d2, d3 };
    for(int i = 0; i < N; i++)
        if(!B[i])
            for(int p = 0; p < 24; p++)
                if(match(i, p, A, D, d))
                    return B[i] = true;
    return false;
}

void solve() {
    // Find the corner cube (lower left front)
    find(C[0], -1, C[2], -1, C[4], -1, -1, -1, -1, 2);

    // Iterate along the x axis edge from there
    while(find(C[0], -1, C[2], -1, '.', '.', -1, -1, -1, 2));
    find(C[0], -1, C[2], -1, -1, C[5], -1, -1, -1, 2);
    auto cr = P[res.back().i][res.back().p];
    auto dy = cr.D[2], dz = cr.D[0];

    c[1] = c[2] = 0;
    S[0].push_back(c[0] = dz);
    // Iterate along the z-axis edge
    while(find('.', '.', C[2], -1, C[4], -1, -1, -1, -1, 0));
    find(-1, C[1], C[2], -1, C[4], -1, -1, -1, -1, 0);

    c[0] = c[2] = 0;
    S[1].push_back(c[1] = dy);
    // Iterate along the y-axis edge
    while(find(C[0], -1, '.', '.', C[4], -1, -1, -1, -1, 1));
    find(C[0], -1, -1, C[3], C[4], -1, -1, -1, -1, 1);

    // Accumulate the slice widths into coordinates of each slices
    std::vector<int> cs[3] = { { 0 }, { 0 }, { 0 } };
    for(int i = 0; i < 3; i++)
        for(int j = 1; j < S[i].size(); j++)
            cs[i].push_back(cs[i][j-1] + S[i][j-1]);

    // Iterate over the slices over each dimension to find a piece that fits
    for(int zi = 0; zi < S[0].size(); zi++) {
        for(int yi = 0; yi < S[1].size(); yi++) {
            for(int xi = 0; xi < S[2].size(); xi++) {
                c[0] = cs[0][zi], c[1] = cs[1][yi], c[2] = cs[2][xi];
                if(bool(c[0]) + bool(c[1]) + bool(c[2]) <= 1)
                    continue;

                // This piece will have colors on it if it's part of an edge
                char ce[6] = { '.', '.', '.', '.', '.', '.' }; // Start out with no colors
                for(int i = 0; i < 3; i++) {
                    if(!c[i])
                        ce[i*2] = C[i*2]; // Add a color if we're at the front
                    if(c[i] == cs[i].back())
                        ce[i*2+1] = C[i*2+1]; // .. or back (or up or right)
                }
                find(ce[0], ce[1], ce[2], ce[3], ce[4], ce[5], S[0][zi], S[1][yi], S[2][xi], -1);
            }
        }
    }
}

int main() {
    // Calculate all the permutations of the cube
    int p = 0;
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            perms[p++] = tip[i]*rot[j];
    for(int i = 0; i < 4; i++)
        for(int j = 1; j < 4; j+=2)
            perms[p++] = tip[i]*rot[j]*tip[1];

    // Read the input
    scanf("%*d %*d %*d %s %d", C, &N);
    for(int i = 0; i < N; i++) {
        scanf("%d %d %d %s", &P[i][0].D[0], &P[i][0].D[2], &P[i][0].D[4], P[i][0].C);
        for(int j = 1; j < 6; j += 2)
            P[i][0].D[j] = P[i][0].D[j-1];
    }

    // Precalculate all the permutations of the pieces
    for(int i = 0; i < N; i++)
        for(int j = 1; j < 24; j++)
            P[i][j] = perms[j]*P[i][0];

    solve(); // Solve the problem
    char faces[7] = "FBDULR";

    // Sort the output and print it out
    std::sort(res.begin(), res.end(), [] (const result& a, const result& b) { return a.i < b.i; });
    for(int i = 0; i < N; i++) {
        auto perm = perms[res[i].p];
        auto p = P[res[i].i][res[i].p];
        printf("%c %c %d %d %d\n", faces[perm.A[0]], faces[perm.A[2]], res[i].z, res[i].y, res[i].x);
    }
}
