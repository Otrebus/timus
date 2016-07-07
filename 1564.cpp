/* 1564. Counting Ones - http://acm.timus.ru/problem.aspx?num=1564
 *
 * Strategy:
 * Binary search.
 *
 * Performance:
 * O(log^2 n), runs the tests in 0.001s using 248KB memory.
 */

#include <string>
#include <iostream>

using ull = unsigned long long;

ull count(ull n)
{
    ull A[20] = { 1 }, P[20] = { 1 };
    for(int i = 1; i < 20; i++)
    {
        P[i] = P[i-1] * 10;
        A[i] = A[i-1]*10 + P[i];
    }
    ull num = n % 10, res = num > 0;
    for(int i = 1; P[i] <= n; i++)
    {
        auto d = n/P[i]%10;
        res += d*A[i-1] + (d > 1 ? P[i] : d == 1 ? num + 1 : 0);
        num += d*P[i];
    }
    return res;
}

ull binary(ull n)
{
    ull first = 1, last = 1000000000000000000, res;
    while(true)
    {
        ull mid = (last+first)/2;
        res = count(mid);
        if(first >= last)
            break;
        if(res > n)
            last = mid-1;
        else if(res < n)
            first = mid+1;
        else
            last = mid;
    }
    return res == n ? last : -1;
}

int main()
{
    ull N;
    std::cin >> N;
    auto i = binary(N);
    std::cout << ((i == -1) ? "Petr lies" : std::to_string(i));
}