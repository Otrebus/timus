/* 1487. Chinese Football - http://acm.timus.ru/problem.aspx?num=1487
 *
 * Strategy:
 * For each node, create a set of its predecessor nodes (stronger teams). For each query (i, j),
 * check that the union of their predecessor sets is empty (they share no common ancestors).
 *
 * Performance:
 * O(N^2 + QN), runs the tests in 0.64s using 3,192KB memory.
 */

#include <stdio.h>
#include <vector>
#include <bitset>

std::vector<int> p[1001]; // Parents
std::bitset<1001> P[1001]; // Predecessor set
bool b[1001]; // Dfs visited?

void dfs(int a, int u) { // Forms the predecessor sets
    if(b[u])
        return;
    b[u] = true;
    for(auto v : p[u]) {
        P[a][v] = true;
        dfs(a, v);
    }
}

int main() {
    int N, Q, i, j;
    char c[1001];
    scanf("%d", &N);
    for(int i = 1; i <= N; i++) {
        scanf("%s", c);
        for(int j = 1; j <= N; j++)
            if(c[j-1] == '1')
                p[j].push_back(i);
    }
    for(int i = 1; i <= N; i++) {
        std::memset(b, 0, sizeof(b));
        dfs(i, i); // Form predecessor sets
    }
    scanf("%d", &Q);
    while(Q--) {
        scanf("%d %d", &i, &j);
        // If the predecessor sets of j and i is nonempty, output No, else YES
        printf((P[j] & P[i]).any() ? "No\n" : "YES\n");
    }
}
