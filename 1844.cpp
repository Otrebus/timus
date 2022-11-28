/* 1844. Warlord of the Army of Mages - http://acm.timus.ru/problem.aspx?num=1844
 *
 * Strategy:
 * Dynamic programming (see comments).
 *
 * Performance:
 * O(nm), runs the test cases in 0.062s using 3,136KB memory.
 */

#include <stdio.h>
#include <cstring>

const int N = 1002;

bool Q[N][N], P[N][N]; // If at some action a mage is in a cabinet
bool A[N][N]; // Whether this action position combination is possible

struct op { // An action
    char c; // + or -
    int x;  // The number of the mage
} v[N], w[N];

bool solve(int n, int m, int k) {

    std::memset(A, 0, sizeof(A));
    std::memset(Q, 0, sizeof(Q));
    A[0][0] = true;

    // Calculate the cabinet contents at any given action for each master
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < k; j++)
            Q[i+1][j] = Q[i][j];
        Q[i+1][v[i+1].x] = v[i+1].c == '+';
    }
    for(int i = 0; i < m; i++) {
        for(int j = 0; j < k; j++)
            P[i+1][j] = P[i][j];
        P[i+1][w[i+1].x] = w[i+1].c == '+';
    }

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            bool either = false;

            // Check whether either master can move from this action, meaning the mage
            // he picks has to be either not in his cabinet or in that of the opponent
            either |= (A[i+1][j] |= A[i][j] && (!P[j][v[i+1].x] || !Q[i+1][v[i+1].x]));
            either |= (A[i][j+1] |= A[i][j] && (!Q[i][w[j+1].x] || !P[j+1][w[j+1].x]));

            // If neither master can make an action from A[i][j], we fail
            if(A[i][j] && !either)
                return false;
        }
    }
    return true;
}

int main() {
    int t, n, m, k;
    char c;
    scanf("%d", &t);
    while(t--) {
        scanf("%d %d %d", &n, &m, &k);

        for(int i = 1; i <= n; i++)
            scanf(" %c %d", &v[i].c, &v[i].x);
        for(int i = 1; i <= m; i++)
            scanf(" %c %d", &w[i].c, &w[i].x);

        printf(solve(n, m, k) ? ":-)\n" : ":-(\n");
    }
}
