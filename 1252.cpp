/* 1252. Sorting the Tombstones - http://acm.timus.ru/problem.aspx?num=1252
 *
 * Strategy:
 * For each number in the input sequence, note its distance from its sorted position. Any k that
 * divides this position allows the number to be transposed into its correct position. Since we
 * want this to be the case for all numbers, we find the greatest common divisor of all these
 * position differences.
 *
 * Performance:
 * O(N log N), runs the test cases in 0.046s using 1220KB; the fastest solution of the compiler.
 */

#include <stdio.h>
#include <algorithm>
#include <stdio.h>
#include <functional>

std::pair<int, int> A[130000];

int gcd(int a, int b)
{
    while(b != 0)
        std::swap(a, b), b = b % a;
    return a;
}

template<typename T> int solve(int N, T order)
{
    std::sort(A, A+N, order);
    int g = 0;
    for(int i = 0; i < N; i++)
        g = gcd(std::abs(A[i].second-i), g);
    return g > 0 ? g : N;
}

int main()
{
    int N;
    scanf("%d", &N);
    for(int i = 0; i < N; i++)
    {
        int x;
        scanf("%d", &x);
        A[i] = { x, i };
    }
    int g1 = solve(N, std::less<std::pair<int, int>>());
    int g2 = solve(N, std::greater<std::pair<int, int>>());
    printf("%d\n", std::max(g1-1, g2-1));
    return 0;
}