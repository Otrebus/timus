/* 2115. The Knowledge Day - http://acm.timus.ru/problem.aspx?num=2115
 *
 * Strategy:
 * List all elements that are in the wrong order. If there are more than a certain number of them,
 * we definitely can't fix the order. If there's only a few, then try swapping all combinations
 * of pairs of them until we have an ordered array.
 *
 * Performance:
 * O(n), runs the tests in 0.046s using 2,952KB memory.
 */

#include <stdio.h>
#include <algorithm>
#include <vector>

int A[100002], B[100002], n, j; // Input, reversed input

std::vector<int> wrong(int* a) { // Returns all indices of elements in the wrong order
    a[0] = -1;
    a[n+1] = 1e9+1;
    std::vector<int> ret;
    for(int i = 2; i <= n; i++) {
        if(a[i] < a[i-1]) {
            // In case of consecutive duplicates, return the first and the last
            for(j = i-1; a[j-1] == a[j]; j--);
            ret.push_back(j);
            for(j = i; a[j+1] == a[j]; j++);
            ret.push_back(j);
        }
    }
    return ret;
}

bool print(bool reverse, int i, int j) { // Prints the swap
    if(reverse)
        i = 1+n-i, j = 1+n-j;
    return printf("Yes\n%d %d", std::min(i, j), std::max(i, j)), true;
}

bool solve(bool reverse) { // Solves the problem for the array or reversed array
    auto a = reverse ? B : A;
    std::vector<int> w = wrong(a);

    if(w.size() > 4)
        return false;

    // Brute-force swap all the pairs and check if any resulting sequence is ordered
    for(auto i : w) {
        for(auto j : w) {
            std::swap(a[i], a[j]);
            if(wrong(a).size() == 0)
                return print(reverse, i, j);
            std::swap(a[j], a[i]);
        }
    }
    return false;
}

int main() {
    scanf("%d", &n);
    for(int i = 1; i <= n; i++)
        scanf("%d", &A[i]), B[1+n-i] = A[i];

    if(wrong(A).size() == 0 || wrong(B).size() == 0)
        printf("Nothing to do here");

    else if(!solve(false) && !solve(true))
        printf("No hope");
}
