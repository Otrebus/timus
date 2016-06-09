/* 1619. Big Brother - http://acm.timus.ru/problem.aspx?num=1619
 *
 * Strategy:
 * Dynamic programming. Denote the solution for a given x = K, y = M by A(x, y). This number equals
 * the fraction of the total amount of paths that we can reach the coordinate (x, y) from the origin
 * without crossing the line x = y. If we denote the total number of paths from the origin to (x, y)
 * by B(x, y), then B(x, y) =  B(x-1, y) + B(x, y-1) = C(x + y, y) where C is the binomial
 * coefficient. The number of valid paths is A(x-1, y)B(x-1, y) + A(x, y-1)B(x, y-1). If we denote
 * this by A1B1 + A2B2, then the answer is (A1B1+A2B2)/(B1+B2). Now, it turns out that B1 = (x/y)B2
 * (expand the binomial coefficients), so the above equation becomes ((x/y)*A1+A2)/(1 + (x/y)) which
 * is the formula we use.
 *
 * Performance:
 * O(max(K, M)^2), runs in 0.795s using 560KB memory. For a faster solution, look up "Bertrand's
 * ballot problem".
 */

#include <functional>
#include <iomanip>
#include <vector>
#include <iostream>
#include <algorithm>
#include <map>

const int maxn = 10000;

double A[maxn+1], B[maxn+1]; // We don't need to store all the A(x, y), just the last two columns
double* a, *b;

int main()
{
    int x, y, n, xmax = 0;
    std::cin >> n;
    std::vector<std::pair<int, int>> input, sortedInput;
    std::map<std::pair<int, int>, double> ans;
    for(int i = 0; i < n; i++)
    {
        std::cin >> x >> y;
        input.push_back({x, y});
        xmax = std::max(x, xmax);
    }
    sortedInput = input; // For a fast assignment of solutions to the input while we compute it all
    sortedInput.push_back({100000, 100000}); // Dummy value in case n = 0 since we call back() below
    std::sort(sortedInput.begin(), sortedInput.end(), std::greater<std::pair<int, int>>());

    a = A; b = B;
    for(int x = 0; x <= xmax; x++)
    {
        for(int y = 0; y <= std::min(x+1, maxn); y++)
        {
            double alpha = double(x)/double(y);
            a[y] = y > x ? 0 : !y ? 1 : (alpha*b[y] + a[y-1])/(1+alpha);
            while(sortedInput.back() == std::make_pair(x, y))
                ans[{x, y}] = a[y], sortedInput.pop_back();
        }
        while(sortedInput.back().first == x)
            sortedInput.pop_back();
        std::swap(a, b);
    }
    for(int i = 0; i < n; i++)
        std::cout << std::setprecision(10) << ans[input[i]] << "\n";
}