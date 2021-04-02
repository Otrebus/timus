/* 1895. Steaks on Board - http://acm.timus.ru/problem.aspx?num=1895
 *
 * Strategy:
 * Dynamic programming over minute and number of steaks fried once and twice cumulative at that
 * minute. From this state, the exact assignment of the flips to each stake is greedily implicit -
 * at any point with a certain number of steaks cooked once and twice, the steaks with the earliest
 * deadline were cooked most thoroughly.
 *
 * Performance:
 * O(tn^3) where t is the minute of the last meal, runs the tests in 0.031s using 26,724KB memory.
 * The memory usage could perhaps be improved by storing only the most recent minute of the DP and
 * for each element store its cooking history in something akin to a linked list.
 */

#include <iostream>

const int maxt = 1001, maxp = 51, inf = 0x3f3f3f3f;

int A[maxt][maxp][maxp]; // DP state
int* P[maxt][maxp][maxp]; // Predecessor matrix to reconstruct the optimal path and state changes
int S[maxt]; // Number of steak cooking periods that have started so far
int E[maxt]; // Same but ended

int x, k, n;

int main() {

    std::memset(A, 0x3f, sizeof(A));
    std::cin >> x >> k >> n;

    int e = 0, t;
    for(int i = 1; i <= n; i++) {
        std::cin >> t;
        E[t]++; // Endpoint of the steak cooking period is when it is to be eaten
        S[std::max(0, t-x)]++; // Start point is end point minus x
        e = std::max(e, t); // Time of the last meal
    }
    
    // Accumulate the steak cooking periods
    int u = 0, d = 0;
    for(int i = 0; i <= e; i++) {
        S[i] += u;
        u = S[i];
        E[i] += d;
        d = E[i];
    }

    // Update the DP state from minute to minute
    A[0][0][0] = 0;
    for(int t = 0; t < e; t++) {
        for(int p = 0; p <= S[t]; p++) { // Number of total steaks, =< cooking periods so far
            for(int p1 = 0; p1 <= p; p1++) { // Number of steaks fried on one side
                int p2 = p-p1; // Number of steaks fried on two sides

                if(A[t][p1][p2] == inf)
                    continue;

                // If we decide to cook this minute, cook maximally
                for(int p1n = 0; p1n <= k; p1n++) { // Number of new steaks fried once

                    int p2n = std::min(k - p1n, p1); // Number of new steaks fried twice
                    int tp1 = p1 + p1n - p2n, tp2 = p2 + p2n; // Total numbers as a result

                    // We can't start cooking more steaks than start endpoints, and make sure
                    // we finish the steaks in time
                    if(tp1 + tp2 > S[t] || tp2 < E[t+1])
                        continue;

                    // Update the DP state for the next minute
                    if(1 + A[t][p1][p2] < A[t+1][tp1][tp2]) {
                        A[t+1][tp1][tp2] = 1 + A[t][p1][p2];
                        P[t+1][tp1][tp2] = &A[t][p1][p2];
                    }
                }

                // If we decide not to cook, same thing, check constraints and update DP state
                if(p1+p2 > S[t] || p2 < E[t+1])
                    continue;

                if(A[t][p1][p2] < A[t+1][p1][p2]) {
                    A[t+1][p1][p2] = A[t][p1][p2];
                    P[t+1][p1][p2] = &A[t][p1][p2];
                }
            }
        }
    }

    int ans = A[e][0][n];
    std::cout << (ans == inf ? -1 : ans) << std::endl;
    if(ans == inf)
        return 0;

    // Recreate the steaks cooked using the predecessor matrix
    auto p = &P[e][0][n];
    std::pair<int, int> out[51];
    for(int t = e, p1 = 0, p2 = n; t || p1 || p2; ) {
        auto a = P[t][p1][p2] - &A[0][0][0];

        int m = maxp, m2 = maxp*maxp;
        // Turn the pointer difference into matrix indices
        int tn = (a / m2), p1n = ((a % m2) / m), p2n = ((a % m2) % m);

        // Greedily deduce which steaks were cooked at this instant, of steaks twice fried
        for(int i = p2n + 1, d2 = p2 - p2n; d2--;)
            out[i++].second = tn;

        // The remaining steaks were fried on one side
        for(int i = p2n + p1n + 1, d1 = p1 + p2 - p1n - p2n; d1--;)
            out[i++].first = tn;

        t = tn, p1 = p1n, p2 = p2n; // Iterate back the previous position in the predecessor matrix
    }

    for(int i = 1; i <= n; i++)
        std::cout << out[i].first << " " << out[i].second << std::endl;
}
