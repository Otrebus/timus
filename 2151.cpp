/* 2151. Mahjong - http://acm.timus.ru/problem.aspx?num=2151
 *
 * Strategy:
 * Brute-force.
 * 
 * Performance:
 * O(sv2^h), where s is the number of suites, v is the number of values and h is the size of the
 * hand, runs the tests in 0.015s using 112KB memory.
 */

#include <stdio.h>
#include <algorithm>

char T[14];
char S[14];

int findProg(int m, int i, int K, int inc) {
    // Tries to apply a progression for hand m at card i for K cards with stepsize inc
    int k = 1;
    int M = m;
    for(int j = i+1; j < 14; j++) {
        if(!(m & (1 << j)) && S[j] == S[i] + inc*k) {
            k++;
            M |= (1 << j);
        }
        if(k == K)
            return M;
    }
    return 0;
}

bool dfs(int m, int t, int p, int i) {
    // Finds a winning hand for hand m from card i onwards with t triplets and p pairs
    int M = 0;
    if(t == 4 && p == 1)
        return true;
    if(i == 14)
        return false;
    if(m & (1 << i))
        return dfs(m, t, p, i+1);

    bool result = false;
    if(p < 1) {
        M = findProg(m, i, 2, 0); // Pair
        if(M)
            result = result || dfs(M, t, p+1, i+1);
    }
    if(t < 4) {
        M = findProg(m, i, 3, 0); // Triplet
        if(M)
            result = result || dfs(M, t+1, p, i+1);
        M = findProg(m, i, 3, 1); // Sequence
        if(M)
            result = result || dfs(M, t+1, p, i+1);
    }
    return result;
}

int main() {
    for(int i = 0; i < 14; i++) {
        char c[3];
        scanf("%s", c);
        T[i] = (c[0]-'0') + (c[1]-'a')*20;
    }
    std::memcpy(S, T, 14);
    std::sort(S, S+14);

    if(dfs(0, 0, 0, 0)) { // Hand is winning
        printf("Tsumo");
        return 0;
    }

    // No winning combo, brute-force new cards and see if we get something that helps
    for(int i = 0; i < 14; i++) {
        char org = T[i];
        for(int s = 0; s < 3; s++) {
            for(int v = 1; v < 10; v++) {
                T[i] = s*20 + v;
                std::memcpy(S, T, 14);
                std::sort(S, S+14);
                if(dfs(0, 0, 0, 0)) {
                    printf("Tenpai");
                    return 0;
                }
            }
        }
        T[i] = org;
    }
    printf("Noten");
}
