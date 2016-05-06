/* 1071. Nikifor 2 - http://acm.timus.ru/problem.aspx?num=1071
 *
 * Strategy:
 * Brute-force.
 *
 * Performance:
 * O(nlog n) where n = max(x,y). Runs the test cases in 0.015s using 260KB memory.
 */

#include <iostream>
#include <algorithm>

int A[32];
int B[32];

int toRadix(int num, int r, int* A)
{
    int i = 0;
    do
    {
        A[i++] = num % r;
        num /= r;
    } while(num);
    return i;
}

bool isSub(int* A, int a, int* B, int b)
{
    while(a >= 0)
    {
        if(A[a] == B[b])
            a--, b--;
        else
            a--;
        if(b == -1)
            return true;
    }
    return false;
}

int main()
{
    int a, b;
    std::cin >> a >> b;
    for(int r = 2; r <= std::max( { 2, a, b } ); r++)
    {
        if(isSub(A, toRadix(a, r, A)-1, B, toRadix(b, r, B)-1))
        {
            std::cout << r;
            return 0;
        }
    }
    std::cout << "No solution";
}