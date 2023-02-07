/* 1275. Knights of the Round Table - http://acm.timus.ru/problem.aspx?num=1275
 *
 * Strategy:
 * If we pick a specific number of goblets to carry to/from one knight and his Kth neighbor, then
 * in order to reach F goblets for each knight in the chain (which consists of every K'th knight
 * until we wrap around back to the first), the rest of the necessary trainsactions are fully
 * determined. To solve the problem, we can then, for each chain, brute-force the number of goblets
 * to assign the knight at the start of the chain that optimally yields F goblets for each knight
 * in the chain.
 *
 * Performance:
 * O(n), runs the tests in 0.046s using 124KB memory.
 */

#include <stdio.h>
#include <cmath>
#include <vector>

int A[1000];
bool B[1000];
int N, K, F;

const int inf = 30001;

std::vector<std::pair<int, int>> v; // Optimal number of goblets to carry to first knights
std::vector<std::pair<int, int>> w; // Each transaction of the optimal solution

int calc(int i, int g1, bool generate) {
    // Calculates the number of goblets carried to+from all knights in a chain if we assign g1
    // to the first knight in order to provide F goblets to each knight
    int g = g1, j = i, sum = 0;
    do {
        B[j] = true;
        sum += std::abs(g);
        if(generate)
            w.push_back({ j, g });
        j = (j+K)%N; // The next knight
        g = F-(A[j]+g); // The number we need to give to the next knight
    } while(j != i && sum < inf);
    return g != g1 ? inf : sum; // The final transaction is to the first knight again; must fit
}

int main() {
    scanf("%d %d %d", &N, &K, &F);
    for(int i = 0; i < N; i++)
        scanf("%d", &A[i]);

    int tot = 0;
    for(int i = 0; i < N; i++) {
        int m = inf, ming = 0;
        if(!B[i]) { // If we haven't done this knight as part of an earlier chain
            for(int g = -inf; g <= inf; g++) // Brute-force the starting number of goblets
                if(int res = calc(i, g, false); res < m)
                    ming = g, m = res;
            v.push_back({ i, ming }); // Save the optimal gobleting
            tot += std::abs(m);
        }
    }

    for(auto [i, k] : v)
        calc(i, k, true);

    if(tot >= inf)
        return printf("-1"), 0;

    // Print out the optimal solution, with the + direction first
    printf("%d\n", tot);
    for(int s = 1; s >= -1; s -= 2)
        for(auto [i, k] : w)
            if(s*k > 0)
                for(int j = 0; j < std::abs(k); j++)
                    printf("%d %d %c\n", 1+i, 1+(i+K)%N, ',' - s);
}
