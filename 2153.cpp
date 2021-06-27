/* 2153. Perfect Squad - http://acm.timus.ru/problem.aspx?num=2153
 *
 * Strategy:
 * Brute-force dfs with memoization.
 *
 * Performance:
 * O(n*2^s) where s is the number of classes, runs the tests in 0.015s using 128KB memory.
 */

#include <stdio.h>

// All possible combinations squads form, the combination each person is, memo array
int A[16], S[16], B[16][16];

char m['W'+1];
int n, i, out = 0, max = 0;

int cnt(int s) {
    // Popcount of s
    int res = 0;
    for(int i = 0; i < 4; i++)
        res += bool(s & (1 << i));
    return res;
}

void dfs(int i, int s) {
    // Finds every way of forming a squad (populates A)
    A[s] = s;
    if(B[i][s] || i >= n)
        return;
    for(int j = 0; j < 4; j++)
        dfs(i+1, s | ((1 << j) & S[i]));
    B[i][s] = 1;
}

int main() {
    m['W'] = 1, m['M'] = 2, m['C'] = 4, m['R'] = 8;

    scanf("%d", &n);
    for(int i = 0, l = 0; i < n; i++, l = 0) {
        scanf("%s", m); // m doubles as input

        // Combine this persons classes into one value
        for(int j = 0; m[j]; j++)
            l |= m[m[j]];
        S[i] = l;
    }

    dfs(0, 0);
    for(int i = 0; i < 16; i++) // Find the size of the biggest squad
        max = cnt(A[i]) > max ? cnt(A[i]) : max;

    printf("%d\n", max);
    if(max < 4) {
        // For any possible squad we can form with our maximum number of members, we add the
        // classes we lack to the set of classes we need
        for(int i = 0; i < 15; i++)
            if(cnt(A[i]) == max)
                out |= ~A[i];

        const char* c[] = { "warrior", "mage", "cleric", "rogue" };
        printf("Looking for");
        for(int i = 0; i < 4; i++)
            if(out & (1 << i))
                printf(" %s", c[i]);
    }
}
