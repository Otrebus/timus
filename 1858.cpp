/* 1858. Magic Cube - http://acm.timus.ru/problem.aspx?num=1858
 *
 * Strategy:
 * At any given point when we build the cube, there is always at least one cell in the unbuilt
 * cube with three known sides (we start in one empty corner cell which has three zero sides), so we
 * index the crystals by their faces so we can quickly find any crystal that fits such a position.
 * When we add the crystal, we then update the known facing sides of the neighboring empty cells
 * and repeat.
 *
 * Performance:
 * O(n^3), runs the tests in 0.125s using 11,204KB memory.
 */

#include <stdio.h>
#include <unordered_map>
#include <vector>
#include <array>
#include <cmath>

struct rot { // A rotation permutation
    int A[6];
    rot() { }
    rot(std::initializer_list<int> v) {
        int i = 0;
        for(auto x : v)
            A[i] = x, i++;
    }
    rot(int A[6]) {
        std::memcpy(this->A, A, sizeof(A)*6);
    }
    rot operator* (const rot& r) const { // Performs the rotation
        rot B;
        for(int i = 0; i < 6; i++)
            B.A[i] = r.A[A[i]];
        return B;
    }
} P[24];

struct crystal {
    int p, S[6]; // Position, sides
} C[30*30*30];

int B[30*30*30][6]; // All the cube sides
int l, n, n2, n3, m, a, b;

std::unordered_multimap<int, int> M; // Maps sides to cubes that contain them
std::vector<int> D; // The set of positions in the cube with 3 known sides

// The three rotations of a cube
rot R[3] = { { 4, 5, 2, 3, 1, 0 }, { 0, 1, 4, 5, 3, 2 }, { 2, 3, 1, 0, 4, 5 } };

bool valid(int p1, int p2) {
    // Returns true if p1 and p2 are neighbors
    return p1 >= 0 && p2 >= 0 && (p1%n == p2%n) + (p1/(n2) == p2/(n2)) + (p1/n%n == p2/n%n) == 2
                   && 0 <= p2 && p2 < n3;
}

bool matches(int A[6], int B[6]) {
    // Checks whether the cube A matches cube B, disregarding unknown positions
    auto b = true;
    for(int i = 0; i < 6; i++)
        if(B[i] != -1)
            b &= A[i] == B[i];
    return b;
}

void update(const crystal& c, rot r, int p) { // Updates position p rotated r with crystal c
    std::memcpy((void*)c.S, r.A, sizeof(r.A));
    
    const int w[3] = { n2, 1, n };
    for(int i = 0; i < 3; i++) { // For each dimension
        for(int s = 0; s < 2; s++) { // For each direction
            auto p2 = p + w[i]*(s ? 1 : -1); // Find the cell in that direction
            if(valid(p, p2)) { // If there's a cell there
                if(B[p2][i*2+(s+1)%2] == -1) { // If we haven't filled it yet
                    B[p2][i*2+(s+1)%2] = c.S[i*2+s]; // Assign the side opposite of crystal c
                    int r = 0;
                    for(int j = 0; j < 6; j++)
                        r += B[p2][j] >= 0;
                    if(r > 2) // If we have 3 sides filled, this is an assignable cell
                        D.push_back(p2);
                }
            }
        }
    }
}

int main() {
    std::memset(B, 0xff, sizeof(B)); // The cube is all unknown at first
    scanf("%d %d", &l, &n);
    n2 = n*n, n3 = n2*n;
    bool init = false;

    // Generate all the 24 permutations of the cube
    int r = 0;
    rot r1 = { 0, 1, 2, 3, 4, 5 };
    auto r2 = r1;
    for(int i = 0; i < 4; i++, r2 = R[0]*r2) {
        auto C = r2;
        for(int j = 0; j < 4; j++, C = R[1]*C, r++)
            P[r] = C;
    }
    r2 = R[2]*r1;
    for(int i = 0; i < 2; i++, r2 = R[2]*R[2]*r2) {
        auto C = r2;
        for(int j = 0; j < 4; j++, C = R[1]*C, r++)
            P[r] = C;
    }

    // Read the input
    for(int i = 0; i < n3; i++) {
        C[i].p = -1;
        for(int k = 0; k < 6; k++) {
            scanf("%d", &C[i].S[k]);
            if(C[i].S[k])
                M.insert({ C[i].S[k], i });
        }

        // Find the initial place with 3 known sides
        for(int j = 0; j < 24 && !init; j++) {
            auto v = P[j]*C[i].S;
            if(v.A[4] == 0 && v.A[2] == 0 && v.A[0] == 0) {
                // We insert that position into the list of known positions
                std::memcpy(B[0], v.A, sizeof(v.A));
                D.push_back(0);
                init = true;
            }
        }
    }
    
    // Mark the exterior sides of the cube 0
    const int w[3] = { n2, 1, n };
    for(int i = 0; i < n3; i++)
        for(int j = 0; j < 3; j++)
            for(int s = 0; s < 2; s++)
                if(!valid(i, i + w[j]*(s ? 1 : -1)))
                    B[i][j*2+s] = 0;
    
    // While we have cells with three known sides
    while(!D.empty()) {
        auto p = D.back();
        D.pop_back();
        int x = 0;

        // Pick a side from one of the cells that have three sides
        for(int i = 0; i < 6; i++)
            if(B[p][i] > 0)
                x = B[p][i];

        // For all blocks that have that side, check if any matches
        auto rn = M.equal_range(x);
        bool found = false;
        for(auto it = rn.first; it != rn.second && !found; it++) {
            auto r = it->second;
            if(C[r].p < 0) {
                for(int i = 0; i < 24 && !found; i++) {
                    r1 = P[i]*rot(C[r].S);
                    if(matches(r1.A, B[p])) { // If it matches, insert it there
                        M.erase(it);
                        C[r].p = p;
                        update(C[r], r1, p); // Update the position and its neighbors
                        found = true;
                    }
                }
                if(found)
                    break;
            }
        }
    }

    // Output 
    scanf("%d", &m);
    for(int i = 0; i < m; i++) {
        scanf("%d %d", &a, &b);
        auto v1x = C[a-1].p%n, v1y = C[a-1].p/n%n, v1z = C[a-1].p/(n2);
        auto v2x = C[b-1].p%n, v2y = C[b-1].p/n%n, v2z = C[b-1].p/(n2);
        auto sq = [] (int x) { return x*x; };
        printf("%.10lf\n", l*std::sqrt(sq(v1x-v2x) + sq(v1y-v2y) + sq(v1z-v2z)));
    }
}
