/* 1568. Train Car Sorting - http://acm.timus.ru/problem.aspx?num=1568
 *
 * Strategy:
 * Consider the incrementing subsequences of the given starting sequence (call it A[0]). For
 * example A[0] = {6, 5, 2, 4, 1, 3} has such subsequences {6}, {5}, {2, 3}, {4}, {1}. At the
 * sorting step, we can "merge" half of the subsequences with their respective successive greater
 * subsequence by placing the two halves on different tracks and then joining the track containing
 * the smaller subsequences first. For example, from the above sequence and corresponding
 * subsequences, we can put the numbers of the subsequences {1}, {4} and {6} on the left track and
 * {2, 3} and {5} on the right track and thereby form the subsequences {6}, {4, 5}, {1, 2, 3}
 * when we merge the two tracks into the sequence A[1] = { 6, 4, 1, 5, 2, 3 }. Since we halve the
 * number of subsequences in each step it takes about log2(n) steps to end up with a final single
 * subsequence.
 * 
 * Practically, we use an array R which is initialized as the incrementing numbers, in the above
 * example { 1, 2, 3, 4, 5, 6 }, and as we iterate over x : A[k] we let R[x] = R[x-1], so for
 * instance adding 3 forms { 1, 2, 2, 4, 5, 6 }, indicating that 2 and 3 belong to an increasing
 * subsequence. This also lets us map the subsequences to different tracks (using the array P) as
 * we scan over R to see when its content changes.
 * 
 * Performance:
 * O(nlog n), runs the tests in 0.015s using 2,968KB memory.
 */

#include <stdio.h>
#include <vector>
#include <numeric>

std::vector<int> A[20], S[10001];
int R[10002], P[10001], N, x, k, p = 1, i = 1;

int main() {
    for(scanf("%d", &N), i = 0; i < N; i++)
        scanf("%d", &x), A[0].push_back(x);

    for(k = 0; p; k++) {
        for(std::iota(R, R+N, 0), i = 0; i < N; i++)
            x = A[k][i], S[R[x] = R[x-1]].push_back(x); // Identify the subsequences

        for(i = 1, p = 0; i <= N; i++)
            P[i] = (p+=R[i]!=R[i-1])%2; // Calculate the parity of the subsequences

        for(auto x : A[k]) if(!P[x]) A[k+1].push_back(x); // Subseqs with parity 0 go first
        for(auto x : A[k]) if(P[x]) A[k+1].push_back(x);
    }

    for(i = 0, printf("%d\n", k-1); i < k; i++, printf("\n"))
        for(auto x : A[i])
            printf("%d ", x);
}
