/* 1544. Classmates 3 - http://acm.timus.ru/problem.aspx?num=1544
 *
 * Strategy:
 * Change the protocol of the network from a single computer by alternately changing its protocol,
 * and use the computer which leads to the fewest number of changes.
 *
 * Performance:
 * O(N^3), runs the tests in 0.015s using 168KB memory.
 */

#include <vector>
#include <stdio.h>

struct node {
    bool p;
    std::vector<int> v;
} nodes[51];

bool P[51]; // Saves the input to reset the network between each candidate
int n[2], n2[2]; // Number of computers that use the protocol

void dfs(int i) { // Changes the protocols by dfs
    auto& node = nodes[i];
    node.p = !node.p;
    n[node.p]++;
    n[!node.p]--;
    for(auto j : nodes[i].v)
        if(nodes[j].p != node.p)
            dfs(j);
}

int main() {
    int N, M, e, j;

    // Read the input and set up the network
    scanf("%d %d", &N, &M);
    for(int i = 1; i <= N; i++) {
        char c[2];
        scanf("%s", c);
        nodes[i].p = P[i] = c[0] == 'E';
        n2[P[i]] = ++n[P[i]];
    }

    for(int i = 1; i <= M; i++) {
        int a, b;
        scanf("%d %d", &a, &b);
        nodes[b].v.push_back(a);
        nodes[a].v.push_back(b);
    }

    int m = 0, mx = 1000, mi;
    // For each computer, keep changing the protocol and record the computer with the minimum
    // number of changes
    for(int i = 1; i <= N; i++) {
        for(m = 0; n[0] != N && n[1] != N; m++)
            dfs(i);
        if(m < mx)
            mx = m, mi = i;
        std::memcpy(n, n2, sizeof(n));
        for(int j = 1; j <= N; j++)
            nodes[j].p = P[j];
    }

    // Output the alternating changes
    printf("%d\n", mx);
    for(int i = 1; i <= mx; i++)
        printf("%d %c\n", mi, (nodes[mi].p + i) % 2 ? 'E' : 'J');
}
