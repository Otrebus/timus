/* 1770. The Party of Ural Champions - http://acm.timus.ru/problem.aspx?num=1770
 *
 * Strategy:
 * For each configuration (i, j) in the input we can deduce the following:
 * 
 *   * If i = 1 and j = 1, we can't have a road between them because if we put one
 *     billboard one way, there can't be a billboard the other way, making the other actually 0:
 *     a contradiction.
 *   * If i = 0 and j = 1, we can put a road with a billboard heading from j to i, and none the
 *     other direction, and similarly for i = 1 and j = 0. 
 *   * If i > 1 or j > 1, we can't have a road with any number of billboards either way because
 *     it'd be a shortcut which violates the given condition.
 *   * If i = 0, and j = 0, we have a billboardless road going either way.
 * 
 * Note that allowing such maximally permissible direct connections (such as the last one which
 * could also have been without a road without violating the connection locally) won't impact the
 * global solution because these connections would by definition still be able to find that way
 * through any other means, and by extension any other path that uses them.
 * 
 * After we've allocated these local roads, we can then run Floyd-Warshall to count the all-pairs
 * fewest billboards, and check that the input is consistent with the solution.
 *
 * Performance:
 * O(n^3), runs the tests in 0.093s using 976KB memory.
 */

#include <stdio.h>
#include <algorithm>

int A[300][300], B[300][300], C[300][300], n, inf = 1e6;

bool test() { // Check that the solution works
    // Calculate the minimum passed-by billboards
    for(int k = 0; k < n; k++)
        for(int i = 0; i < n; i++)
            for(int j = 0; j < n; j++)
                B[i][j] = std::min(B[i][j], B[i][k] + B[k][j]);

    // Check that the minimum billboard distance is consistent with the input
    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++)
            if(B[i][j] != A[i][j])
                return false;
    return true;
}

int main() {
    scanf("%d", &n);
    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++)
            scanf("%d", &A[i][j]);

    // Allocate the roads as given in the header comment
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            if(A[i][j] > 1 || A[j][i] > 1)
                B[i][j] = inf;
            else if(A[i][j] == 1 && A[j][i] == 1)
                B[i][j] = inf;
            else if(A[i][j] == 1 && !A[j][i])
                B[i][j] = 1;
        }
    }

    // Make a matrix where those roads are jiving with the output format
    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++)
            C[i][j] = (B[i][j] == inf || i == j) ? 0 : B[i][j] + 1;

    if(!test())
        return printf("NO"), 0;
    printf("YES\n");
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++)
            printf("%d", C[i][j]);
        printf("\n");
    }
}
