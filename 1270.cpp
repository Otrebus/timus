/* 1270. Unicube - http://acm.timus.ru/problem.aspx?num=1270
 *
 * Strategy:
 * Make a bipartite graph by connecting every valid rotation from the N cubes to the M cubes, solve
 * the maximum matching problem using Ford-Fulkerson, and check that we matched all M.
 *
 * Performance:
 * O(M(M+N)^2), runs the tests in 0.015s using 624KB memory.
 */

#include <array>
#include <vector>
#include <stdio.h>

const int maxn = 30;
const int mn = maxn*2+2;

struct node {
    bool b; // Visited during dfs?
    std::vector<int> v; // Neighbors
} nodes[mn];

bool V[mn][mn]; // Visibility matrix
bool C[mn][mn]; // Capacity matrix

int rot[24][6] = { // Valid cube rotations
    { 0, 1, 2, 3, 4, 5 },
    { 0, 4, 1, 2, 3, 5 },
    { 0, 3, 4, 1, 2, 5 },
    { 0, 2, 3, 4, 1, 5 },
    { 1, 5, 2, 0, 4, 3 },
    { 1, 4, 5, 2, 0, 3 },
    { 1, 0, 4, 5, 2, 3 },
    { 1, 2, 0, 4, 5, 3 },
    { 5, 3, 2, 1, 4, 0 },
    { 5, 4, 3, 2, 1, 0 },
    { 5, 1, 4, 3, 2, 0 },
    { 5, 2, 1, 4, 3, 0 },
    { 3, 0, 2, 5, 4, 1 },
    { 3, 4, 0, 2, 5, 1 },
    { 3, 5, 4, 0, 2, 1 },
    { 3, 2, 5, 4, 0, 1 },
    { 4, 1, 0, 3, 5, 2 },
    { 4, 5, 1, 0, 3, 2 },
    { 4, 3, 5, 1, 0, 2 },
    { 4, 0, 3, 5, 1, 2 },
    { 2, 1, 5, 3, 0, 4 },
    { 2, 0, 1, 5, 3, 4 },
    { 2, 3, 0, 1, 5, 4 },
    { 2, 5, 3, 0, 1, 4 }
};

int A[30][6]; // Input cubes (N)
int N, M;

void add(int a, int b) { // Links a pair of nodes in the flow graph
    V[a][b] = V[b][a] = true;
    nodes[a].v.push_back(b);
    nodes[b].v.push_back(a);
    C[a][b] = true;
}

void add(int a, int S[6]) {
    // Adds one of the M cubes, and adds to the graph if it fits one of the N cubes
    for(int i = 0; i < N; i++) {
        for(int r = 0; r < 24; r++) { // Try every rotation
            bool match = true;
            for(int j = 0; j < 6 && match; j++)
                if(S[j] != 0 && S[j] != A[i][rot[r][j]])
                    match = false; // If side color is not wildcard and doesn't match
            if(match)
                add(1+i, 1+N+a); // If we do match, add to graph
        }
    }
}

bool dfs(int u) { // Finds an augmenting path
    nodes[u].b = true;
    if(u == N+M+1) // End node found
        return true;
    for(auto v : nodes[u].v)
        if(!nodes[v].b && C[u][v] && dfs(v)) // Augmenting path found
            return C[u][v] = false, C[v][u] = true;
    return false;
}

int main() {
    // Read the N cubes
    scanf("%d", &N);
    for(int i = 0; i < N; i++)
        for(int j = 0; j < 6; j++)
            scanf("%d", &A[i][j]);

    // Read the M cubes and match them to the previous
    scanf("%d", &M);
    for(int i = 0; i < M; i++) {
        int A[6];
        scanf("%*d %*d %*d");
        for(int i = 0; i < 6; i++)
             scanf("%d", &A[i]);
        add(i, A);
    }

    // Add the source edges
    for(int i = 0; i < N; i++)
        add(0, i+1);

    // Add the sink edges
    for(int i = 0; i < M; i++)
        add(1+N+i, N+M+1);

    // Ford-Fulkerson
    for(int aug = true; aug; aug = dfs(0))
        for(int i = 0; i < 2+M+N; i++)
            nodes[i].b = false;

    int out[30];

    // If there's still capacity in any of the sink edges, we didn't match them all
    for(int i = 0; i < M; i++)
        if(C[N+1+i][N+M+1])
            return printf("0"), 0;
    
    // Populate the output from the intermediate edges with flow (no capacity)
    for(int i = 0; i < N; i++)
        for(int j = 0; j < M; j++)
            if(V[1+i][1+N+j] && !C[1+i][1+N+j])
                out[j] = i;

    for(int i = 0; i < M; i++)
        printf("%d\n", 1+out[i]);
}

// The program that generated the cube permutations:

// int rot0[6] = { 0, 1, 2, 3, 4, 5 };
// int rotA[6] = { 1, 5, 2, 0, 4, 3 };
// int rotB[6] = { 4, 1, 0, 3, 5, 2 };
// int rotC[6] = { 0, 4, 1, 2, 3, 5 };
// 
// struct rot {
//     std::array<int, 6> A;
//     rot() {}
//     rot(int B[6]) {
//         for(int i = 0; i < 6; i++)
//             A[i] = B[i];
//     }
//     rot operator* (const rot& r) {
//         rot x;
//         for(int i = 0; i < 6; i++) {
//             x.A[i] = r.A[A[i]];
//         }
//         return x;
//     }
// };
// 
// int main() {
//     rot r0(rot0);
// 
//     std::vector<std::array<int, 6>> out;
//     
//     // Rotate fully along the X axis, and for each, rotate along the Y axis
//     rot r1 = r0;
//     for(int i = 0; i < 4; i++, r1 = rot(rotA)*r1) {
//         rot r2 = r1;
//         for(int j = 0; j < 4; j++, r2 = rot(rotC)*r2) {
//             out.push_back(r2.A);
//         }
//     }
// 
//     // Rotate 90 degrees left/right to get the remaining two faces to rotate along the Y axis
//     r1 = rot(rotB)*r1;
//     rot r2 = r1;
//     for(int j = 0; j < 4; j++) {
//         out.push_back(r2.A);
//         r2 = rot(rotC)*r2;
//     }
//     r1 = rot(rotB)*rot(rotB)*r1;
//     r2 = r1;
//     for(int j = 0; j < 4; j++) {
//         out.push_back(r2.A);
//         r2 = rot(rotC)*r2;
//     }
// 
//     for(auto o : out) {
//         printf("{ %d", o[0]);
//         for(int i = 1; i < o.size(); i++)
//             printf(", %d", o[i]);
//         printf(" },\n");
//     }
// }
