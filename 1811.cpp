/* 1811. Dual-SIM Phone - http://acm.timus.ru/problem.aspx?num=1811
 *
 * Strategy:
 * At least one of the pair of operators must have a total of at least n/2 connected destinations
 * (for the maximum of n = 1e4, k = 1e5, there are at most 20 such operators). For each such
 * an operator, organize those destinations into a segment tree, allowing us to query their maximum
 * cost quickly, and pair it up with every other operator, calculating the maximum cost either by
 * 
 *   a) Going through the numbers of both operators in a linear fashion, or
 *   b) Inserting the cost for each number of operator B into the tree of A,
 * 
 * where for each pair we pick the method expected to be the fastest in practice.
 *
 * Performance:
 * O(k^2/n + (k^2/n^2)log(n)), runs the tests in 0.125s using 3,676KB memory.
 */

#include <vector>
#include <stdio.h>
#include <algorithm>
#include <numeric>

int inf = (1 << 31)-1;
struct edge {
    int y, c; // Calling operator y costs c
};
int n, k, m = inf, x, y, c, S;

int C[10001]; // Used to tabulate and merge the destination costs of operators
std::vector<edge> E[10001]; // The entries of an operator
int W[10001]; // The indices of E sorted by size

struct tree { // A segment tree that allows log n insertion and constant querying of maximum values
    std::vector<int> A;
    
    tree() {}
    tree(std::vector<edge>& v) {
        for(S = 1; S < n*2; S*=2); // Calculate the necessary size of the tree
        A.resize(S);
        std::fill(A.begin()+S/2, A.begin()+S/2+n, inf); // Anything not inserted is infinite
        for(auto e : v) // Insert the values in the bottom row
            A[S/2-1+e.y] = e.c;

        for(int i = S/2-1; i >= 1; i--) // Do max() of the internal nodes
            A[i] = std::max(A[i*2], A[i*2+1]);
    }

    void _insert(int k, int n) { // Inserts value k at position n
        for(A[n] = k, n/=2; n >= 1; n/=2)
            A[n] = std::max(A[n*2], A[n*2+1]);
    }
    void insert(int k, int p) { _insert(k, S/2-1+p); }
    int get(int i) { return A[S/2-1+i]; }
    int max() { return A[1]; }
} T[10001];

int calcA(int a, std::vector<edge>& v) {
    // Calculates the maximum cost of operator a and the entries of another operator with entries v
    // by method a) given in the header
    int M = 0;
    std::fill(C+1, C+1+n, inf);

    for(int i = 1; i <= n; i++)
        C[i] = T[a].get(i);
    for(auto& e : v)
        C[e.y] = std::min(C[e.y], e.c);

    for(int r = 1; r <= n; r++)
        M = std::max(C[r], M);
    return M;
}

int calcB(int a, std::vector<edge>& v) {
    // Calculates the maximum cost of operator a and the entries of another operator with entries v
    // by method b) given in the header
    std::vector<std::pair<int, int>> p;
    for(auto e : v) {
        if(e.c < T[a].get(e.y)) {
            p.push_back({ e.y, T[a].get(e.y) });
            T[a].insert(e.c, e.y);
        }
    }
    int ans = T[a].max();
    for(auto r : p)
        T[a].insert(r.second, r.first);
    return ans;
}

int calc(int a, int b) {
    // Calculates the cost of combining operator a and b - if they're within an order of magnitude
    // sizewise, we just do a linear merge, otherwise we insert into the tree of a from b
    return (E[b].size() > S/10) ? calcA(a, E[b]) : calcB(a, E[b]);
}

int main() {
    scanf("%d %d", &n, &k);

    for(int i = 0; i < k; i++) {
        scanf("%d %d %d", &x, &y, &c);
        E[x].push_back({ y, c });
    }

    // Sort the operators internally by destination
    for(int i = 1; i <= n; i++)
        std::sort(E[i].begin(), E[i].end(), [] (auto a, auto b) { return a.y < b.y; });

    // Sort the operators by size, store the sorted indices in W
    std::iota(W, W+n, 1);
    std::sort(W, W+n, [] (auto i, auto j) { return E[i].size() > E[j].size(); });

    // Create the trees for big enough operators
    for(int i = 0; i < n && E[W[i]].size() >= n/2; i++)
        T[W[i]] = tree(E[W[i]]);

    // For each pair of sufficiently big operators, calculate the maximum cost
    for(int a = 0; a < n && E[W[a]].size() >= n/2; a++)
        for(int b = a+1; b < n && E[W[a]].size()+E[W[b]].size() >= n; b++)
            m = std::min(m, calc(W[a], W[b]));

    if(m == inf)
        return printf("No solution"), 0;
    printf("%d", m);
}
