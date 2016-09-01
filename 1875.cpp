/* 1875. Angry Birds - http://acm.timus.ru/problem.aspx?num=1875
 *
 * Strategy:
 * For each pair of points (x1, y1) and (x2, y2), solve the system of equations
 * 
 *    y1 = a*x1^2 + b*x1
 *    y2 = a*x2^2 + b*x2
 * 
 * for a and b, remove all points that fit on this curve, recurse with the remaining points,
 * and report the smallest number of recursions.
 *
 * Performance:
 * Constant (since the input is of constant size), runs the tests in 0.015s using 312KB memory.
 */

#include <algorithm>
#include <iostream>
#include <vector>

int solve(std::vector<std::pair<long long, long long>> v)
{
    if(v.empty())
        return 0;
    int ret = v.size();
    for(int i = 0; i < v.size(); i++)
    {
        for(int j = i + 1; j < v.size(); j++)
        {
            auto x1 = v[i].first, y1 = v[i].second, x2 = v[j].first, y2 = v[j].second;
            auto w = v;
            for(auto it = w.begin(); it != w.end(); )
            {
                auto x = it->first, y = it->second;
                auto cmp = [] (long long a, long long b) { return a > 0 ? b < 0 : b > 0; };
                // Check that we're dealing with a parabola at all, if the point fits on the
                // parabola, and that the parabola bends downwards
                if(y*(x1*x1*x2 - x1*x2*x2) == (x2*y1 - x1*y2)*x*x + (x1*x1*y2 - x2*x2*y1)*x
                   && cmp(x2*y1-x1*y2, x1*x1*x2-x1*x2*x2))
                    it = w.erase(it);
                else
                    it++;
            }
            if(v.size() == w.size())
                continue;
            ret = std::min(ret, 1 + solve(w));
        }
    }
    return ret;
}

int main()
{
    std::vector<std::pair<long long, long long>> v;
    for(int i = 0; i < 5; i++)
    {
        int x, y;
        std::cin >> x >> y;
        v.push_back( { x, y } );
    }
    std::cout << solve(v);
}