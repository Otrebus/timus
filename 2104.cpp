/* 2104. Game with a Strip - http://acm.timus.ru/problem.aspx?num=2104
 *
 * Strategy:
 * Minimax.
 *
 * Performance:
 * O(n), Runs the tests in 0.031s using 5,012KB memory.
 */

#include <stdio.h>

char A[1000001]; // The input array
int S[1000001]; // The cumulative number of A's

int dfs(int s, int a, int b) { 
    // Calculates whether player s wins a strip between indices a and b

    int n = b-a, d = S[b] - S[a];
    if(d == n || d == 0) // An array full or empty of A's wins or loses
        return d == n ? 1 : 2;
    if(n/2%2) // Uneven strip, draw
        return 0;
    
    auto x1 = dfs(1+s%2, a, (a+b)/2); // Fold one direction
    auto x2 = dfs(1+s%2, (a+b)/2, b); // Fold the other
    
    // Return the best result we can achieve from either folds
    return (x1 == s || x2 == s) ? s : (x1 == 0 || x2 == 0) ? 0 : 1+s%2;
}

int main() {
    int n, i = 0;
    scanf("%d", &n);
    for(int s = 0; s < 2; s++)
        scanf("%s", A+s*n);
    for(int i = 1; i <= 2*n; i++)
        S[i] = S[i-1] + (A[i-1] == 'A');

    int a = dfs(1, 0, n*2);
    printf(a == 0 ? "Draw" : a == 1 ? "Alice" : "Bob");
}
