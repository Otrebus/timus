/* 2107. Oppa Funcan Style - http://acm.timus.ru/problem.aspx?num=2107
 *
 * Strategy:
 * The transitions form a graph, and only graphs containing either cycles or paths are valid (other
 * paths have first positions that can't be reached). Brute-force merge the paths of the graph into
 * disjoint cycles until the prime factors of the length of each cycle fit inside the prime factors
 * of k. Meaning, if the length of a cycle has some prime factor p^i, then k must have a factor p^j
 * where j >= i.
 *
 * Performance:
 * Hard to derive the complexity, runs the tests in 0.015s using 164KB memory.
 */

#include <stdio.h>
#include <vector>

struct dancer {
    int N, R, v; // Next dancer, number of dancers entering this position, visited
} D[36];

int p[11] = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31 };

std::vector<int> v[36]; // The paths by length 
std::vector<std::vector<int>> out; // All the paths
// Head of each path, Length of each path, Prime powers of k, nUmber of paths of each length, answer
int H[36], L[36], P[36], U[36], ans[36];
int s, g; // Number of different lengths of paths, number of paths

int length(int f, int g, int i) { // Returns the length of a path
    if(D[i].v)
        return 0;
    D[i].v = true;
    if(D[i].N)
        return 1 + length(f, g, D[i].N);
    return 0;
}

bool check(int len) { // Checks that the prime factors of a cycle fit k
    for(int r = 0; r < 11; r++) {
        int j = 0; // Power of the prime
        while(len > 1 && len % p[r] == 0) {
            j++;
            len /= p[r];
        }
        if(j > P[r])
            return false;
    }
    return true;
}

bool dfs(int);

bool dfs2(int i, int d, int n, int* u) {
    // Picks a number of cycles of each length to be merged into one cycle
    // Args are (number of paths of this length we use, total number paths used so far,
    //           paths left in total, path lengths used) all applying for one cycle
    if(i == s) {
        if(!d) // We didn't choose any paths at all
            return false;
        
        // Check if the prime factors of the length of the resulting cycle fit the ones in k
        int sum = 0;
        for(int k = 0; k < s; k++)
            sum += u[k]*(L[k] + 1);
        if(!check(sum))
            return false;

        // Push the answer into the vector, recurse into the next cycle
        std::vector<int> o;
        for(int i = 0; i < s; i++)
            o.push_back(u[i]);
        out.push_back(o);
        if(dfs(n))
            return true;
        out.pop_back(); // Next cycle didn't work, pop the answer, try another one
        return false;
    }

    // For each number of paths of length L[i], try adding that path to the cycle and recurse
    for(int j = 0; j <= U[i]; j++) {
        u[i] = j;
        U[i] -= j;
        if(dfs2(i+1, d+j, n-j, u))
            return true;
        U[i] += j;
    }
    return false;
}

bool dfs(int n) {
    // With n cycle sizes left to fill, finds some solution
    if(!n)
        return true;
    int u[36]; // The number of paths of each length that we're trying out
    std::memset(u, 0, sizeof(u));
    return dfs2(0, 0, n, u);
}

int main() {
    int n, k, x;
    scanf("%d %d", &n, &k);
    for(int i = 1; i <= n; i++) { // Read the input
        scanf("%d", &x);
        D[i].N = x; // Next index
        D[x].R++; // Number of paths leading into this (should be one max)
    }

    int K = k;
    for(int i = 0; i < 11; i++) // Extract the prime factors from k
        while(K % p[i] == 0)
            K /= p[i], P[i]++;

    for(int i = 1; i <= n; i++) { // Find every path in the input
        if(D[i].R > 1)
            return printf("No"), 0;
        if(!D[i].R && !D[i].v) { // Classify the paths by length
            H[g] = i;
            v[length(i, g, i)].push_back(g);
            g++;
        }
    }

    for(int i = 1; i <= n; i++) { // Check that any cycle has a valid length
        if(!D[i].v) {
            int sum = 0, j = i;
            do {
                j = ans[j] = D[j].N;
                sum++;
            } while(j != i);
            if(!check(sum))
                return printf("No"), 0;
        }
    }

    // Compress the length->paths array so we don't have to loop over 35 entries
    for(int i = 0; i <= 35; i++) {
        if(!v[i].empty()) {
            L[s] = i;
            v[s++] = v[i];
        }
    }

    for(int i = 0; i < g; i++)
        U[i] = v[i].size();

    if(!dfs(g)) // Brute-force merge paths into cycles
        return printf("No"), 0;

    for(auto& o : out) { // For each set of paths (actually lengths of paths) that will be merged
        int last = 0, first = 0;
        for(int i = 0; i < s; i++) { // For each length of path in the answer
            if(!first && o[i])
                first = H[v[i].back()];

            for(int k = 0; k < o[i]; k++) { // Merge all the paths of that length
                ans[last] = x = H[v[i].back()];
                v[i].pop_back();
                do {
                    ans[x] = D[x].N;
                    last = x;
                    x = D[x].N;
                } while(x && x != first);
            }
        }
        ans[last] = first;
    }

    printf("Yes\n");
    for(int i = 1; i <= n; i++)
        printf("%d ", ans[i]);
}
