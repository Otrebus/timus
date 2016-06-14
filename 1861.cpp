/* 1861. Graveyard in Deyja - http://acm.timus.ru/problem.aspx?num=1861
 *
 * Strategy:
 * Dynamic programming. Let A[n][k] be the subsolution for a string of length n whose suffix is the
 * prefix of size k of the lich name. Iterate n from 1 to the full length, at each step using the
 * value of the answer at the previous position n-1 and its k's.
 *
 * Performance:
 * O(n^2), finishes the tests in 0.015s using 4,108KB memory.
 */

#include <vector>
#include <algorithm>
#include <cstring>
#include <stdio.h>

const int maxn = 1001;

int A[maxn][maxn];
char input[maxn];
char key[maxn];

int main()
{
    scanf("%s %s", input, key);
    int N = strlen(input), K = strlen(key);

    std::vector<int> suf; // All the suffixes that are also prefixes of the lich name
    for(int i = 0; i < K; i++)
        if(strncmp(key, key+K-1-i, i+1) == 0)
            suf.push_back(i+1);

    std::memset(A, 0xFF, sizeof(A));
    A[0][0] = 0;
    for(int n = 1; n <= N; n++)
    {
        // In case of a full match, we update the answers for all the prefixes that are
        // suffixes of the match at this position
        if(A[n-1][K-1] >= 0 && (input[n-1] == '?' || input[n-1] == key[K-1]))
            for(auto s : suf)
                A[n][s] = A[n-1][K-1] + 1;
        // The answer for 0 matched elements will simply be the maximum of the number of
        int max = A[n-1][0]; // matched elements of the previous string length
        for(int k = 1; k <= K; k++)
        {   // Check if we can extend the matches of the previous string position
            if(A[n-1][k-1] >= 0 && (input[n-1] == '?' || input[n-1] == key[k-1]))
                A[n][k] = std::max(A[n-1][k-1], A[n][k]);
            max = std::max(A[n][k], max);
        }
        A[n][0] = max;
    }
    printf("%d", A[N][0]);
}
