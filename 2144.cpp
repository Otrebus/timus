/* 2144. Cleaning the Room - http://acm.timus.ru/problem.aspx?num=2144
 *
 * Strategy:
 * Check that the boxes are sorted internally, attempt to sort them together, and check if
 * the final sequence is sorted.
 *
 * Performance:
 * O(nk + nlog n), runs the tests in 0.015s using 160KB memory.
 */

#include <stdio.h>
#include <algorithm>

int n, k, K[100];
struct box { int l, r; } B[100];

int main() {
    scanf("%d", &n);
    for(int i = 0; i < n; i++) {
        scanf("%d", &k);
        for(int i = 0; i < k; i++)
            scanf("%d", &K[i]);
        for(int i = 1; i < k; i++) // Check that the're internally sorted
            if(K[i-1] > K[i])
                return printf("NO"), 0;
        B[i].l = K[0], B[i].r = K[k-1]; // Assign the leftmost and rightmost elements
    }
    std::sort(B, B+n, [] (box& a, box& b) { return a.l < b.r; }); // Sort
    for(int i = 1; i < n; i++) // Check that we're sorted
        if(B[i-1].r > B[i].l)
            return printf("NO"), 0;
    printf("YES");
}
