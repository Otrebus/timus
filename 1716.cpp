/* 1716. Alternative Solution - http://acm.timus.ru/problem.aspx?num=1716
 *
 * Strategy:
 * Dynamic programming. Let the element A[n][k][y] denote the number of correct answers in all
 * answer sequences that are n elements long, with k "YES" elements, that end with a "YES" if
 * y = 1, and "NO" if y = 0. We can calculate the number of elements in A[n][k][1] by:
 *
 *  a) adding a "YES" element (and hence a correct answer) to all sequences in A[n-1][k-1][1] and
 *     multiplying this with the ratio of all sequences of n elements and k "YES"'es that happen
 *     to end in a "YES".
 *  b) adding a "YES" element (and hence an incorrect answer) to all sequences in A[n-1][k-1][0] and
 *     multiplying this with the ratio of all sequences of n elements and k "YES"'es that happen
 *     to end in a "NO".
 *
 * The number of elements in A[n][k][0] is computed similarly. Also note that we only need to store
 * the last two elements of n in A, to save memory.
 *
 * Performance:
 * O(n^2), runs the tests in 0.436s using 460KB memory.
 */

#include <iostream>
#include <algorithm>
#include <iomanip>

double A[2][5001][2];

int main()
{
    int N, S;
    std::cin >> N >> S;
    int b = S - 2*N, a = N - b;
    A[1][1][1] = 1;
    for(int n = 2; n <= N; n++)
    {
        for(int k = 0; k <= std::min(n, b); k++)
        {
            if(k > 0)
                A[n%2][k][1] = (A[(n-1)%2][k-1][1]+1)*(k-1)/(n-1) + A[(n-1)%2][k-1][0]*(n-k)/(n-1);
            A[n%2][k][0] = (A[(n-1)%2][k][0] + 1)*(n-1-k)/(n-1) + A[(n-1)%2][k][1]*k/(n-1);
        }
    }
    // We actually wanted the number of incorrect solutions
    std::cout << std::setprecision(10) << N - (A[N%2][b][1]*b/N + A[N%2][b][0]*(N-b)/N) << "\n";
}