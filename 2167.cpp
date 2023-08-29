/* 2167. Cipher Message 5 - http://acm.timus.ru/problem.aspx?num=2167
 *
 * Strategy:
 * Naive. For each input, xor it with an incrementing number until a prime number is produced.
 *
 * Performance:
 * Roughly O(Tlog T), runs the tests in 0.062s using 1,208KB memory.
 */

#include <memory>
#include <stdio.h>

const int N = 1100000;
bool P[N+1]; // Composite numbers

void calc() { // Calculates the composite numbers up to N
    for(int p = 2; p*p <= N; p++)
        if(!P[p])
            for(int i = p*p; i <= N; i += p)
                P[i] = true;
    P[1] = P[0] = true;
}

int main() {
    calc();

    int T, a;
    scanf("%d", &T);
    while(T--) {
        scanf("%d", &a);
        for(int i = 0; ; i++) {
            if(!P[a^i]) {
                printf("%d\n", i);
                break;
            }
        }
    }
}
