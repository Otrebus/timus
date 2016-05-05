/* 1375. Bill Clevers - http://acm.timus.ru/problem.aspx?num=1375
 *
 * Strategy:
 * For each 0 <= x < p: calculate what y^2 mod p would have to be to satisfy the given equation.
 * Check if there is such a y from a precalculated table.
 *
 * Performance:
 * O(p), runs in 0.046s using 4168KB memory.
 */

#include <iostream>
#include <cstring>

const int maxp = 1000000;
int r[maxp]; // The aforementioned table

int main()
{
    long long k, p;
    std::cin >> k >> p;
    std::memset(r, 0xFF, sizeof(r));
    for(long long i = 0; i < p; i++)
        r[(i*i)%p] = i;
    for(long long x = 0; x < p; x++)
    {
        int rem = (p*x+k-x*x)%p; // The aforementioned calculation
        if(r[rem] == -1)
            continue;
        std::cout << x << " " << r[rem];
        return 0;
    }
    std::cout << "NO SOLUTION";
}