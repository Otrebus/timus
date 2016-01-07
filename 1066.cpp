/* 1066. Garland - http://acm.timus.ru/problem.aspx?num=1066
 *
 * Strategy:
 * Do binary search over the space of all B where in each iteration we solve the given system of
 * linear equations.
 *
 * Performance:
 * O(N) if we consider the upper bound of the binary search to be a constant, runs the test
 * suite in 0.015s using 476KB memory.
 */

#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <iomanip>

double a[1000]; // The diagonal of the matrix
double b[1000]; // The constant vector

double A;
int N;

// Solves the system of equations using Gaussian elimination, returns the minimum element of
// the solution
double eval(double B)
{
    a[0] = a[1] = a[N-1] = 2;
    b[0] = -2*A;
    b[1] = 2 + -A;
    b[N-1] = -2*B;
    for(int i = 1; i < N-2; i++)
    {
        double x = 1.0/a[i];
        a[i+1] = 2.0 - x;
        b[i+1] = 2.0 + x*b[i];
    }
    double min = std::numeric_limits<double>::infinity();
    for(int i = N-1; i > 1; i--)
        b[i-1] += (1.0/a[i])*b[i];
    for(int i = 0; i < N; i++)
        min = std::min(min, -b[i]/a[i]);
    return min;
}

double search(double l, double r) // Binary search
{
    while(true)
    {
        double mid = l + (r-l)/2;
        double result = eval(mid);
        if(std::abs(r-l) < 1e-7)
            return mid;
        else if(result >= 0)
            r = mid;
        else
            l = mid;
    }
}

int main()
{
    std::cin >> N >> A;
    std::cout << std::fixed << std::setprecision(2) << search(0, 1e50);
}