/* 1747. Kingdom Inspection - http://acm.timus.ru/problem.aspx?num=1747
 *
 * Strategy:
 * Denote the answer A[n]. From the first point a, we can choose the next point b in n-1 different
 * ways. From there, we can proceed in a few different ways.
 * 
 *  a) We continue the rest of the journey as if b is the first point, and we can travel among the
 *     remaining points (except a), ending up at b, in A[n-1] different ways, before returning to
 *     point a.
 *  b) After one of the interior (n-2)*2 points in each sequence of a) (except the last one) we can
 *     return to point b, and then continue along our journey to the original next point in the
 *     sequence (returning to a at the end). This equals ((n-2)*2-1)*A[n-1] different journeys.
 *  c) After the first occurrence of some point x after b, we can return to point b and then back
 *     to point x (and again, at the end, skipping b and heading straight to a). This is equivalent
 *     to B[n-1] different journeys, where B[n] is the number of journeys we can make with a
 *     repeating (paired) point in it.
 * 
 * The value of B[n] can be calculated by taking a sequence A[n-1] but adding a pair of points
 * somewhere in the interior 2*(n-1)-1 points of the sequence, with that point taking on n-1
 * different values (re-valuing the others), totalling (A[n-1]*(2*(n-1)-1)*(n-1)) different
 * journeys, or by taking a sequence which already has a repeating pair, B[n-1], and adding a
 * pair between the repeating pair, with m-1 different values (B[n-1]*(n-1)).
 * 
 * We end up with the recurrence
 * 
 *   A[n] = (n-1)*(A[n-1] + A[n-1]*((n-2)*2-1) + B[n-1]),
 *   B[n] = A[n-1]*(2*(n-1)-1)*(n-1) + B[n-1]*(n-1)
 * 
 * which we can solve as a single expression in A, namely
 * 
 *   A[n] = (n-1)*(A[n-1]*(n*2-3) + A[n-2]*(n-2)),
 * 
 * which we calculate bottom-up.
 *
 * Performance:
 * O(n), runs the tests in 0.015s using 648KB memory (the memory use could be reduced since we only
 * use the two previous values of A.)
 */

#include <stdio.h>

long long A[100000] = { 0, 0, 0, 2 }, n, p;

int main() {
    scanf("%lld %lld", &n, &p);
    for(long long i = 4; i <= n; i++)
        A[i] = (i-1)*(A[i-1]*(i*2-3)%p + A[i-2]*(i-2)%p)%p;
    printf("%lld", A[n]%p);
}
