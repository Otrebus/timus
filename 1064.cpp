/* 1064. Binary Search - http://acm.timus.ru/problem.aspx?num=1064
 *
 * Strategy:
 * Simulation; essentially this problem asks, for a range of N's, whether it is possible to perform
 * binary search partitions on the range [0, N), to arrive at the given i in L steps. The actual 
 * content of the nondecreasing array does not matter since it is assumed to be whatever results in
 * the given behavior of the binary search that produces the set of partitions.
 *
 * Performance:
 * O(nlog n), runs the test suite in 0.015s using 336KB memory.
 */

#include <iostream>

bool binary(int x, int l, int N)
{
    int p = 0, q = N-1, L = 0;
    while (p <= q && L <= l)
    {
        int i = (p + q)/2;
        L++;
        if(i == x)
            return L == l;
        if (x < i)
            q = i - 1;
        else
            p = i + 1;
    }
    return false;
}

std::pair<int, int> v[10000];

int main()
{
    int i, L, t = 0;
    std::cin >> i >> L;
    for(int n = 1; n <= 10000; n++)
    {
        if(binary(i, L, n))
        {
            if(t > 0 && v[t-1].second == n-1)
                v[t-1].second = n;
            else
            {
                v[t].first = v[t].second = n;
                t++;
            }
        }
    }
    std::cout << t << std::endl;
    for(int i = 0; i < t; i++)
        std::cout << v[i].first << " " << v[i].second << std::endl;
}