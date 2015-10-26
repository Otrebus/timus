/* 1602. Elevator - http://acm.timus.ru/problem.aspx?num=1602
 *
 * Strategy:
 * Straightforward calculation of the total time of each possible choice.
 *
 * Performance:
 * O(n), runs in 0.001s using 452KB memory.
 */

#include <iostream>
#include <algorithm>
#include <vector>

int k, n;
double v, u;
const double eps = 1e-9;

double f(int i)
{
    if(i == 1)
        return u * (n-i);
    double tp = u * (n-i);
    double te = v * (k-1);
    double d = 15.0 - std::min(15.0, (tp > te ? tp - te : 0));
    return std::max(tp, te) + d + 5.0 + 2.0*(i-1)*v;
}

int main()
{
    std::cin >> n >> k >> u >> v;
    std::vector<std::pair<int, double>> v;
    for(int i = 1; i <= n; i++)
        v.push_back({i, f(i)});
    auto it = std::min_element(v.begin(), v.end(), 
        [] (std::pair<int, double>& a, std::pair<int, double>& b) 
           { return std::abs(a.second-b.second) < eps ? a.first > b.first : a.second < b.second; });
    printf("%d\n", (*it).first);
}