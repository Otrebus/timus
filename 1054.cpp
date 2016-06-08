/* 1054. Hanoi Tower - http://acm.timus.ru/problem.aspx?num=1054
 *
 * Strategy:
 * Check the position of the largest disk. If it has been moved to the target position, we know that
 * at least the first half of all moves have been performed. Add this number to the answer and
 * recurse to the appropriate subproblem of size N-1.
 *
 * Performance:
 * O(N), runs in 0.001s using 256KB memory.
 */

#include <iostream>

int A[32];
bool bad = false;

int calc(int n, int from, int to, int temp)
{
    if(n == 0)
        return 0;
    if(A[n-1] == from)
        return calc(n-1, from, temp, to);
    else if(A[n-1] == to)
        return (1 << (n-1)) + calc(n-1, temp, to, from);
    return bad = true;
}

int main()
{
    int N;
    std::cin >> N;
    for(int i = 0; i < N; i++)
        std::cin >> A[i];
    int result = calc(N, 1, 2, 3);
    std::cout << (bad ? -1 : result);
}