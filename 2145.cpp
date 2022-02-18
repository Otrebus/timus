/* 2145. Olympiad for Everyone - http://acm.timus.ru/problem.aspx?num=2145
 *
 * Strategy:
 * Sort the participants by their ability and the problems by their difficulty. Iterate over the
 * participants and the problems in a merge-like manner, to quickly calculate how many problems
 * each person can solve. Then, if the first participant can solve k1 problems, the next participant
 * can solve k2 problems, and so on, the answer is
 * 
 *   k1*(k2-1)*(k3-2)* ... 
 * 
 * with all the operations performed modulo p.
 *
 * Performance:
 * O(nlog n), runs the tests in 0.062s using 856KB memory.
 */

#include <stdio.h>
#include <algorithm>

int A[100000], B[100000], n, p, i, j, m, k;

int main() {
    scanf("%d", &n);
    for(int i = 0; i < n; i++)
        scanf("%d", &A[i]);
    for(int i = 0; i < n; i++)
        scanf("%d", &B[i]);
    scanf("%d", &p);

    std::sort(A, A+n), std::sort(B, B+n);

    long long ans = 1;
    while(i < n || j < n)
        if(j < n && B[j] <= A[i]) // Add problem that can be solved
            m++, j++;
        else // Can't be solved by the current student, iterate to the next
            ans = (ans*(j-i++))%p;
    printf("%lld", ans);
}
