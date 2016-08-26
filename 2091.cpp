/* 2091. Natural Selection - http://acm.timus.ru/problem.aspx?num=2091
 *
 * Strategy:
 * Naive.
 *
 * Performance:
 * O(nm^2), runs the tests in 0.015s using 312KB memory.
 */

#include <algorithm>
#include <iostream>
 
const int maxn = 100, maxm = 100;
 
bool A[maxn][maxm];
 
int main()
{
    int n, m, min = 101;
    std::cin >> n >> m;
    for(int i = 0; i < n; i++)
        for(int j = 0; j < m; j++)
            std::cin >> A[i][j];
    std::pair<int, int> maxij;
    for(int i = 0; i < m; i++)
    {
        for(int j = i + 1; j < m; j++)
        {
            int g[4] = { 0, 0, 0, 0 };
            for(int k = 0; k < n; k++)
            {
                g[0] += (A[k][i] && A[k][j]);
                g[1] += (A[k][i] && !A[k][j]);
                g[2] += (!A[k][i] && A[k][j]);
                g[3] += (!A[k][i] && !A[k][j]);
            }
            if(*std::max_element(g, g+4) < min)
            {
                min = *std::max_element(g, g+4);
                maxij = { i, j };
            }
        }
    }
    std::cout << min << "\n" << (maxij.first + 1) << " " << (maxij.second + 1);
}