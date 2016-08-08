/* 1629. Trip - http://acm.timus.ru/problem.aspx?num=1629
 *
 * Strategy:
 * Straightforward simulation. To avoid some hassle to the detriment of speed, all possible relevant
 * scheduled flights are explicitly calculated for each stop.
 *
 * Performance:
 * O(N(QN + Nlog N)), runs the tests in 0.015s using 280KB memory.
 */

#include <vector>
#include <algorithm>
#include <iostream>

int main()
{
    int N, ans = 0, q, d, t;
    std::cin >> N;
    for(int i = 0; i < N - 1; i++)
    {
        std::vector<int> v;
        char c;
        std::cin >> q >> d >> t;
        while(q--)
        {
            int hh, mm;
            std::cin >> hh >> c >> mm;
            int t = hh*60 + mm;
            for(int i = 0; i <= 2*N; i++) // Add all future flights at this time
                v.push_back(t + 24*60*i + d);
        }
        std::sort(v.begin(), v.end());
        auto f = std::lower_bound(v.begin(), v.end(), ans); // Earliest possible flight
        ans = *f + t;
    }
    std::cout << ans;
}