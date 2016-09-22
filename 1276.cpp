/* 1276. Train - http://acm.timus.ru/problem.aspx?num=1276
 *
 * Strategy:
 * Dynamic programming. Let A[i][j][k][l][a] denote the number of ways to build a train with i
 * "AA" carts, j "AB" carts, k "BA" carts and l "BB" carts that end with an "A" if a = 0 or "B"
 * if a = 1. We can calculate the value of this by "detaching" each type of cart and looking up
 * the number of ways of building a train without that cart - for example, for A[i][j][k][l][0],
 * we end with either "BA" or "AA" so we add together entries A[i][j][k-1][l][1] and
 * A[i-1][j][k][l][0], which is the number of different trains that have one less of either of
 * these two carts. We then add together all trains that have fitting ends and have length K.
 * To get around the memory requirement, we calculate the answer bottom up and only keep track
 * of the last two entries at the index position of i.
 *
 * Performance:
 * O(k^4), runs the tests in 0.001s using 564KB memory.
 */

#include <stdio.h>
#include <algorithm>

unsigned long long A[2][42][42][42][2];
int h[4] = { 1, 1, 1, 1 };

int main()
{
    unsigned long long ans = 0;
    int N, K;
    char s[3];
    scanf("%d %d %s", &N, &K, s);
    K += 4; // To avoid checking for negative indices, we start them at 1 and need to adjust this K
    int last = s[0] - 'A'; // The left attachment of the locomotive
    A[1][1][1][1][(s[1]-'A')] = 1; // Base case: the empty train needs to start with the locomotive
    for(int i = 0; i < N; i++)
    {
        scanf("%s", s);
        h[(s[0]-'A')*2+s[1]-'A']++; // Add to the histogram
    }
    
    for(int i = 1; i <= h[0]; i++)
    {
        int rest = K-i, lim = std::min(rest, h[1]);
        for(int j = 1; j <= lim; j++)
        {
            int rest2 = rest-j, lim2 = std::min(rest2, h[2]);
            for(int k = 1; k <= lim2; k++)
            {
                int rest3 = rest2-k, lim3 = std::min(rest3, h[3]);
                for(int l = (rest2-k == K-3 ? 2 : 1); l <= lim3; l++)
                {
                    // As explained above
                    A[i%2][j][k][l][0] = A[i%2][j][k-1][l][1] + A[(i-1)%2][j][k][l][0];
                    A[i%2][j][k][l][1] = A[i%2][j-1][k][l][0] + A[i%2][j][k][l-1][1];
                    if(l == rest3) // This train is of length K, add to the solution
                        ans += A[i%2][j][k][l][last];
                }
            }
        }
    }
    if(ans)
        printf("YES\n%llu", ans);
    else
        printf("NO");
}