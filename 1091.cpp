/* 1091. Tmutarakan Exams - http://acm.timus.ru/problem.aspx?num=1091
 *
 * Strategy:
 * Dynamic programming. Let the partial solution P(k, i, g) be the solution for sets of size k, 
 * where i is the last integer in the set (of size k) used, and g is the gcd of all members of the
 * set. If we go through all P(k, i, g) where i <= j and g <= j, we find that if we add another
 * member j to each set of the solution represented, we will add P(k, i, g) to the solution of
 * P(k+1, j, gcd(g, j)). Going through all j's for each k, and each k up to K, we arrive at the 
 * total solution.
 *
 * Performance:
 * O(KS^3), runs the tests in 0.062s using 408KB memory.
 */

#include <iostream>
#include <vector>
#include <algorithm>

int gcd(int a, int b)
{
    while(b != 0)
        std::swap(a, b), b = b % a;
    return a;
}

int main()
{
    int K, S, ans = 0;
    std::cin >> K >> S;
    std::vector<std::vector<int>> v1, v2, *a = &v1, *b = &v2;
    for(int i = 0; i < 51; i++)
        v1.push_back(std::vector<int>(51, 0)), v2.push_back(std::vector<int>(51, 0)), v2[i][i] = 1;
    for(int k = 2; k <= K; k++)
    {
        for(int i = 2; i <= S; i++)
            for(int j = i+1; j <= S; j++)
                for(int g = 2; g <= S; g++)
                    (*a)[j][gcd(g, j)] += std::min((*b)[i][g], 10000);
        std::swap(a, b);
        std::fill((*a).begin(), (*a).end(), std::vector<int>(51, 0));
    }
    for(int g = 2; g <= S; g++)
        for(int i = 2; i <= S; i++)
            ans = std::min(10000, ans + (*b)[i][g]);
    std::cout << ans << std::endl;
}