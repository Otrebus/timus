/* 1791. Uncle Styopa and Buses - http://acm.timus.ru/problem.aspx?num=1791
 *
 * Strategy:
 * Greedy - always let through a bus from the queue with the closest upcoming deadline.
 *
 * Performance:
 * O(m+n), runs the tests in 0.062s using 2,500KB memory.
 */

#include <stdio.h>
#include <algorithm>

struct bus {
    int t1, t2, m; // Arrival time, deadline, shortest future deadline
} A[100001], B[100001];
int n, m, t, p;

void read(int& s, bus* R) { // Reads the given bus from the input
    scanf("%d", &s);
    for(int i = 0; i < s; i++)
        scanf("%d %d", &t, &p), R[i] = { t, t + p - 1 };
}

void calc(int s, bus* R) { // Calculates the shortest future deadline for each bus
    R[s].m = 1e9;
    for(int i = s-1; i >= 0; i--)
        R[i].m = std::min(R[i+1].m-1, R[i].t2);
}

int main() {
    read(n, A), read(m, B);
    calc(n, A), calc(m, B);

    int i = 0, j = 0, qi = 0, qj = 0, T = 0; // qi and qj are the front of the bus queues
    while(i < n || j < m || qi != i || qj != j) {
        if(qi == i && qj == j) // If the queues are empty (no waiting buses, advance the time)
            T = std::min(A[i].t1, B[j].t1);

        // Fill the queues with the buses who arrived at this time
        while(A[i].t1 == T)
            i++;
        while(B[j].t1 == T)
            j++;

        if(qj != j || qi != i) { // Some queue isn't empty
            bus* b;
            if(qj == j) // Queue j is empty, let through a bus from the other queue
                b = &A[qi++];
            else if(qi == i) // Vice versa
                b = &B[qj++];
            else // Buses in both, pop the queue with the closest future deadline
                b = (A[qi].m - T) <= (B[qj].m - T) ? &A[qi++] : &B[qj++];
            if(b->t2 < T) // We missed the deadline
                return printf("NO"), 0;
            T++; // Advance the time; takes one minute to transit
        }
    }
    printf("YES");
}
