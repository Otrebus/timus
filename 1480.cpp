/* 1480. Coupons - http://acm.timus.ru/problem.aspx?num=1480
 *
 * Strategy:
 * Assume a solution, put the largest number in the upper left corner, then go through the matrix
 * row by row and assign to the current position in the matrix the largest element that, along with
 * both the left value or the value above the element, satisfies the solution. Different solutions
 * were tried for each matrix size until a minimum was found, which turned out to be the one given
 * in the code.
 *
 * Performance:
 * O(N^2 log N), running the tests in 0.015s using 372KB memory.
 */

#include <stdio.h>
#include <algorithm>
#include <set>
#include <functional>

int A[50][50];

int main()
{
    std::set<int, std::greater<int>> s;
    int N;
    scanf("%d", &N);
    for(int i = 1; i < N*N; i++)
        s.insert(i);
    A[0][0] = N*N;
    int sol = N*N + N/2 + 1;
        printf("%d\n%d ", sol, N*N);
    for(int i = 0; i < N; i++)
    {
        for(int j = (!i ? 1 : 0); j < N; j++)
        {
            int max = std::max(j - 1 >= 0 ? A[i][j-1] : 0, i - 1 >= 0 ? A[i-1][j] : 0);
            auto it = s.lower_bound(sol-max);
            A[i][j] = *it;
            s.erase(it);
            printf("%d ", A[i][j]);
        }
        printf("\n");
    }
    std::system("pause");
}