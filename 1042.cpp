/* 1042. Central Heating - http://acm.timus.ru/problem.aspx?num=1042
 *
 * Strategy:
 * Gaussian elimination.
 *
 * Performance:
 * O(N^3), runs the tests in 0.062s using 552KB memory.
 */

#include <iostream>

int A[250][251];

int main()
{
    int n, x = 0;
    std::cin >> n;
    for(int i = 0; i < n; i++)
    {
        while(true)
        {
            std::cin >> x;
            if(x == -1)
                break;
            A[x-1][i] = A[x-1][n] = 1;
        }
    }
    for(int i = 0; i < n; i++)
    {
        int k = i;
        for(; k < n; k++) // Find the pivot row
            if(A[k][i] == 1)
                break;
        for(int j = 0; j < n + 1; j++) // Bring it to the proper position
            std::swap(A[k][j], A[i][j]);
        for(k = 0; k < n; k++)
            if(A[k][i] && k != i) // Clear out rows above and below the pivot row
                for(int j = i; j < n + 1; j++)
                    A[k][j] ^= A[i][j];
    }
    for(int i = 0; i < n; i++)
        if(A[i][n])
            std::cout << (i + 1) << " ";
}