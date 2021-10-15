/* 1670. Asterisk - http://acm.timus.ru/problem.aspx?num=1670
 *
 * Strategy:
 * Maintain a stack of sequences, onto which we push each new input number as a singleton sequence.
 * After each input number, check the top two sequences on the stack. If they can be merged into a
 * sequence - either back-to-back or when swapped - replace them with a new node on the stack
 * representing that sequence and the way we formed it (reversed with an asterisk, or standard),
 * with the two merged sequences as parents, and check again. If there's a single item on the stack
 * after all the input, print the tree formed by that item as root.
 *
 * Performance:
 * O(N), runs the tests in 0.015s using 964KB memory.
 */

#include <stdio.h>

short N, j, s, x;
// Seq is { left, right endpoints of sequence, my index, parent indices, is reversed }
struct seq { short a, b, i, p1, p2; bool r; };

seq B[20001], S[10001]; // Sequence node repository, stack

void dfs(int i) {
    // Recursively prints the node
    printf("(");
    if(B[i].p1 == i) {
        printf("%hu", B[i].a);
    } else {
        dfs(B[i].p1);
        printf(B[i].r ? "," : "*");
        dfs(B[i].p2);
    }
    printf(")");
}

int main() {
    scanf("%d", &N);

    for(int i = 0; i < N; i++) {
        scanf("%hu", &x);
        S[s++] = B[j] = { x, x, j, j, j, false };
        j++;

        while(s >= 2) {
            auto v = S[s-1], u = S[s-2]; // The top two sequences on the stack
            bool fits = v.a == u.b+1, reverse = v.b == u.a-1;
            if(fits || reverse) {
                // The top two sequences on the stack form a sequence, reversed or back-to-back
                auto x = fits ? u : v, y = fits ? v : u;
                S[s-=2] = (B[j] = { x.a, y.b, j, x.i, y.i, fits });
                j++, s++;
            } else
                break;
        }
    }
    if(s != 1)
        return printf("IMPOSSIBLE"), 0;
    dfs(S[0].i);
}
