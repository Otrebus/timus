/* 1699. Turning Turtles - http://acm.timus.ru/problem.aspx?num=1699
 *
 * Strategy:
 * Build a tree from the given maze, associate each node with the number of turns from the root to
 * each node, and, for each query, find the LCA l of the two given points (using RMQ) and add the
 * number of turns from a to l and from b to l.
 *
 * Performance:
 * O(hw + qlog(hw)), runs the tests in 0.171s using 31,116KB memory.
 */

#include <vector>
#include <stdio.h>

#pragma comment(linker, "/STACK:33554432")

const int maxn = 100001;

int A[maxn*10]; // The segment tree for the RMQ
int Ed[maxn*5]; // Maps each euler number to a depth
                // (I call the numbers associated with the Euler walk 'euler numbers')
int E[maxn]; // Maps each node to its euler number
int Ei[maxn*5]; // Maps each euler number to its node
char s[maxn]; // The input
int e = 1; // Current euler number when doing the Euler tour
int D = 1; // Smallest square that contains the lowest row in the segment tree
int h, w, tx = -1, ty = -1;

struct node {
    int x, y, t; // Position, number of turns from the root
    bool v; // Visited
    std::vector<std::pair<int, int>> out; // Outgoing edges and their associated euler numbers
} nodes[100001];

int build(int i, int L, int R) {
    // Builds the segment tree, associating index i with an (or the) euler number with the smallest
    // associated depth among the range of euler numbers from L to R
    int mid = (L+R)/2;
    if(L < R) {
        int m1 = build(i*2, L, mid);
        int m2 = build(i*2+1, mid+1, R);
        A[i] = Ed[m1] < Ed[m2] ? m1 : m2;
    }
    return A[i];
}

void build() {
    // Builds the segment tree
    std::memset(A, 0, sizeof(A));
    while(D < e-1)
        D*=2; // The index of the start of the bottom row of the tree
    for(int x = 1, i = D; i < D + e-1; i++)
        A[i] = x++; // The bottom row is just all the euler numbers we generated
    build(1, 1, D);
}

int findMin(int i, int l, int r, int L, int R) {
    // RMQ to find an euler number between l and r with a minimal depth
    if(l <= L && r >= R)
        return A[i];
    
    int mid = (L+R)/2, m1 = 0, m2 = 0;
    if(l <= mid)
        m1 = findMin(i*2, l, r, L, mid);
    if(r > mid)
        m2 = findMin(i*2+1, l, r, mid+1, R);
    return Ed[m1] < Ed[m2] ? m1 : m2;
}

int lca(int l, int r) {
    // Between euler numbers l and r, returns an euler number with a minimal depth
    return findMin(1, std::min(l, r), std::max(l, r), 1, D);
}

void addEuler(int i, int d) {
    // Associates an euler number to the node i at depth d
    Ed[e] = d;
    Ei[e] = i;
    e++;
}

void dfs(int X, int Y, int fx, int fy, int T, int d) {
    // Builds a node at this position (fx, fy = direction we came from, T = turns so far, d = depth)
    int i = Y*w + X;
    nodes[i].v = true;
    E[i] = e;
    addEuler(i, d); // Associate the root with an euler number

    // Check every direction from this node for another node
    for(auto& dr : { std::pair{ 0, 1 }, { 1, 0 }, { -1, 0 }, { 0, -1 } }) {

        int dx = dr.first, dy = dr.second;
        int nx = X + dx, ny = Y + dy;
        int j = ny*w + nx;
        if(nx >= 0 && ny >= 0 && ny < h && nx < w && !nodes[j].v) {
            // This node is a turn if we move in another direction than we came from
            nodes[j].t = T + ((fx || fy) && (dx != fx && dy != fy));
            nodes[i].out.push_back({e, j}); // Associate each edge with an euler number
            addEuler(i, d); // Associate that euler number with this node and depth
            dfs(nx, ny, dx, dy, nodes[j].t, d+1);
        }
    }
}

int main() {
    int m;
    std::memset(Ed, 0x3f, sizeof(Ed));
    scanf("%d %d", &h, &w);
    for(int y = 0; y < h; y++) {
        scanf("%s", s);
        for(int x = 0; x < w; x++) {
            auto& node = nodes[x+y*w];
            node.x = x, node.y = y;
            if(s[x] != '#') // Pretend we already visited empty spots
                node.v = true;
            else
                tx = x, ty = y; // Root of the tree
        }
    }

    dfs(tx, ty, 0, 0, 0, 0);
    build();

    scanf("%d", &m);
    while(m--) {
        int x1, y1, x2, y2;
        scanf("%d %d %d %d", &y1, &x1, &y2, &x2);
        int a = (y1-1)*w+x1-1, b = (y2-1)*w+x2-1;

        int mi = Ei[lca(E[a], E[b])]; // The index of the LCA of a and b

        // Find the child of the LCA on the path to node a by finding the edge with the largest
        // euler number that is smaller than the euler number of a (and do the same for b)
        int a2 = mi, b2 = mi;
        for(auto& p : nodes[mi].out) {
            if(p.first < E[a])
                a2 = p.second;
            if(p.first < E[b])
                b2 = p.second;
        }
        auto na = nodes[a], na2 = nodes[a2], nb = nodes[b], nb2 = nodes[b2];
        // The number of turns is the number of turns between a and b to the corresponding closest
        // child of the LCA (calculated above), and add one if there is a turn between the children
        printf("%d\n", na.t - na2.t + nb.t - nb2.t + (na2.x != nb2.x && na2.y != nb2.y));
    }
}
