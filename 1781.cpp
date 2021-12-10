/* 1781. Clean Code - http://acm.timus.ru/problem.aspx?num=1781
 *
 * Strategy:
 * Iterate over the columns from the left to the right. For each such a column at position j, swap
 * it with a column at position k > j that when swapped according to the rules of the problem brings
 * all the ones above the main diagonal at j.
 *
 * Performance:
 * O(n^3), runs the tests in 0.015s using 200KB memory.
 */

#include <stdio.h>
#include <vector>
#include <algorithm>

int A[100][100], n;

int main() {
    scanf("%d", &n);
    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++)
            scanf("%d", &A[i][j]), A[i][j];

    std::vector<std::pair<int, int>> ans;
    for(int i = 0; i < n; i++) { // For each diagonal (or row)
        for(int j = i+1; j < n; j++) { // For each other higher row/column
            int V[100]; // The entries in the associated column if perform the swap
            for(int k = 0; k < n; k++)
                V[k] = A[k][j];
            std::swap(V[i], V[j]);

            int mv = 0; // The maximum position of a 1
            for(int i = 0; i < n; i++)
                if(V[i])
                    mv = i;

            if(mv <= i) { // If the above mentioned position is above the diagonal, perform the swap
                for(int k = 0; k < n; k++)
                    std::swap(A[i][k], A[j][k]);
                for(int k = 0; k < n; k++)
                    std::swap(A[k][i], A[k][j]);
                ans.push_back({i, j});
                break;
            }
        }
    }

    // If the resulting matrix still doesn't cut it, report a failure
    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++)
            if(A[i][j] && i > j)
                return printf("-1"), 0;

    // Print the output
    printf("%d\n", ans.size());
    for(auto a : ans)
        printf("%d %d\n", 1+a.first, 1+a.second);
    printf("\n");
}
