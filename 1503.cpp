/* 1503. Polynomial - http://acm.timus.ru/problem.aspx?num=1503
 *
 * Strategy:
 * Recursively find the roots of the derivative of the function and do binary search between those
 * points. For multiple roots: if the multiplicity of f'(a) is n, then the multiplicity of a root
 * at f(a) is n+1.
 *
 * Performance:
 * O(N^2), runs the tests in 0.109s using 12,288KB memory. 
 */

#include <vector>
#include <stdio.h>
#include <algorithm>
#include <iostream>
#include <iomanip>

using ld = long double;
const ld inf = std::numeric_limits<ld>::infinity();

std::vector<int> diff(std::vector<int> v) { // Differentiates a polynomial
    std::vector<int> w;
    for(int i = 0, deg = v.size()-1; i < v.size()-1; i++)
        w.push_back(v[i]*deg--);
    return w;   
}

ld eval(std::vector<int> v, ld x) { // Differentiates a polynomial with Horner's rule
    ld sum = 0;
    for(int i = 0; i < v.size(); i++)
        sum = sum*x + v[i];
    return sum;
}

ld bisect(ld x1, ld x2, std::vector<int> v) { // Finds a root of a poly between x1 and x2
    auto a = eval(v, x1), b = eval(v, x2);

    while(x2-x1 > 1e-12) {
        ld mid = x1 + (x2-x1)/2;
        auto r = eval(v, mid);
        if((b > a) ? (r <= 0) : (r >= 0))
            x1 = mid;
        else
            x2 = mid;
    }
    return std::abs(eval(v, x1)) < 1e-4 ? x1 : inf; // Return the root if a root was found
}

std::vector<std::pair<ld, int>> solve(std::vector<int> v) {
    if(v.size() < 2)
        return {};

    // Find roots of the derived function and add a couple fake roots at both extremes
    auto dd = solve(diff(v));
    std::vector<std::pair<ld, int>> d;
    d.push_back({-1e15, 0});
    d.insert(d.end(), dd.begin(), dd.end());
    d.push_back({1e15, 0});

    // Check for roots between the roots of the derivative
    std::vector<std::pair<ld, int>> d1;
    for(int i = 0; i < d.size()-1; i++)
        if(auto x = bisect(d[i].first, d[i+1].first, v); x < inf)
            d1.push_back({ x, 1 });

    // Remove any duplicates
    auto last = std::unique(d1.begin(), d1.end(), [] (auto a, auto b) {
        return std::abs(a.first-b.first) < 1e-3;
    });
    d1.erase(last, d1.end());
    
    // Add the multiplicities
    for(int i = 0; i < d1.size(); i++)
        for(int j = 0; j < d.size(); j++)
            if(std::abs(d1[i].first-d[j].first) < 1e-3)
                d1[i] = { d[j].first, d1[i].second + d[j].second };
    return d1;
}

int main() {
    int n, x;
    std::cin >> n;
    std::vector<int> v;

    for(int i = 0; i < n+1; i++) {
        std::cin >> x;
        v.push_back(x);
    }

    auto w = solve(v);
    for(auto z : w)
        for(int i = 0; i < z.second; i++)
            std::cout << std::fixed << std::setprecision(20) << z.first << std::endl;
}
