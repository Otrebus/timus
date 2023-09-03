/* 2161. Mount & Blade - http://acm.timus.ru/problem.aspx?num=2161
 *
 * Strategy:
 * Calculate the prefix and suffix maximums of the gold-imbued attack and defense strengths for the
 * list of knights, and for each knight check that they can attack the strongest defender and still
 * have enough gold to defend against the strongest attacker.
 *
 * Performance:
 * O(n), runs the tests in 0.093s using 2,868KB memory.
 */

#include <stdio.h>
#include <algorithm>

int n;

struct {
    int a, d, g;
} K[100002]; // Knights

int D[100002][2]; // Suffix/prefix maximums of the defense powers
int A[100002][2]; // Suffix/prefix maximums of the attack powers

bool calc(int k) { // Checks if knight k wins against everyone

    // Check that we can attack against any defending knight to the left or right
    int l = D[k-1][0], r = D[k+1][1]; // Strongest defender on left and right sides
    int kd = K[l].d > K[r].d ? l : r; // Strongest defender
    if(K[k].a < K[kd].d)
        return false;

    // Check that we have enough gold against the strongest attacker, at the same time
    l = A[k-1][0], r = A[k+1][1];
    int ka = K[l].a > K[r].a ? l : r;
    int dg = std::max(K[kd].d - (K[k].a - K[k].g), 0); // What we have to spend attacking
    if(K[k].d - dg <= K[ka].a) // What we have to spend attacking we can't defend with
        return false;

    return true;
}

int main() {
    scanf("%d", &n);

    K[0].a = K[0].d = K[0].g = -1; // Sentinel values
    K[n+1].a = K[n+1].d = K[n+1].g = -1;

    for(int i = 1; i <= n; i++) {
        scanf("%d %d %d", &K[i].a, &K[i].d, &K[i].g);
        K[i].a += K[i].g; // We track the gold-imbued maximums
        K[i].d += K[i].g;
    }

    for(int i = 1; i <= n; i++) { // Calculate the prefix maximums
        A[i][0] = K[i].a > K[A[i-1][0]].a ? i : A[i-1][0];
        D[i][0] = K[i].d > K[D[i-1][0]].d ? i : D[i-1][0];
    }
    for(int i = n; i >= 1; i--) { // Calculate the suffix maximums
        A[i][1] = K[i].a > K[A[i+1][1]].a ? i : A[i+1][1];
        D[i][1] = K[i].d > K[D[i+1][1]].d ? i : D[i+1][1];
    }

    for(int i = 1; i <= n; i++) // For each knight, check if he wins
        if(calc(i))
            return printf("%d", i), 0;
    printf("-1");
}
