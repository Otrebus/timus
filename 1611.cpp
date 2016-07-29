/* 1611. Decimation - http://acm.timus.ru/problem.aspx?num=1611
 *
 * Strategy:
 * Dynamic programming. Let A[m][k] be the optimal result for a string of length m that contains k
 * friends. For each length and number of friends, we can either add a friend or add a conductor,
 * which updates A[m+1][k+1] or A[m+1][k], respectively. The updated result depends on if there is
 * a good conductor or bad conductor at spot m+1, or if a friend ends up there.
 *
 * Performance:
 * O(nk), runs the tests in 0.001s using 6,208KB memory (rated 4/429).
 */

#include <stdio.h>
#include <algorithm>
#include <stack>
#include <limits>

const int maxn = 10000;
const int maxk = 50;

int A[maxn+maxk+1][maxk+1]; // As described above
int head[maxn+maxk+1][maxk+1]; // The position of the last friend
int prev[maxn+maxk+1][maxk+1]; // The position of the previous last friend
char input[maxn+1];

int main()
{
    int n, k;
    scanf("%d %d %s", &n, &k, &input);

    for(int m = 1; m <= n+k; m++)
        for(int j = 0; j <= k; j++)
            A[m][j] = std::numeric_limits<int>::max();
    for(int m = 0; m < n+k; m++)
    {
        // Adding a conductor
        for(int j = std::max(0, m+1-n); j <= std::min(k, m); j++)
        {
            int a = A[m][j] + ((m+1) % 10 == 0 && input[m-j] == '1');
            if(a < A[m+1][j])
            {
                A[m+1][j] = a;
                head[m+1][j] = head[m][j];
                prev[m+1][j] = prev[m][j];
            }
        }
        // Adding a friend
        for(int j = std::max(0, m+1-n); j <= std::min(k-1, m); j++)
        {
            int b = A[m][j] + ((m+1) % 10 == 0);
            if(b < A[m+1][j+1])
            {
                A[m+1][j+1] = b;
                head[m+1][j+1] = m+1;
                prev[m+1][j+1] = head[m][j];
            }
        }
    }

    // Find the best result, from 0 to k friends for the full string
    int min = std::numeric_limits<int>::max(), minm, mink;
    for(int j = 0; j <= k; j++)
    {
        if(A[n+j][j] < min)
        {
            min = A[n+j][j];
            minm = n+j;
            mink = j;
        }
    }

    // Print out the stuff using the head and prev arrays
    printf("%d\n%d ", min, mink);
    std::stack<int> output;
    for(int n = minm, k = mink; k; n = prev[n][k--])
        output.push(head[n][k]);
    for(; !output.empty(); output.pop())
        printf("%d ", output.top());
}