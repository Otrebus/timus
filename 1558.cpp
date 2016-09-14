/* 1558. Periodical Numbers - http://acm.timus.ru/problem.aspx?num=1558
 *
 * Strategy:
 * Brute-force. Calculate the sum of the given numbers for some finite number of digits and then
 * for a small range of non-repeating digits, find the smallest period beyond those.
 *
 * Performance:
 * Something like O(n^3) where n is the maximum length of the input, runs the tests in 0.001s using
 * 276KB memory.
 */

#include <limits>
#include <iostream>
#include <string>

char A[200], B[200], C[200];

int main()
{
    std::string a, b;
    std::cin >> a >> b;
    int n = a.size() - 2;
    for(int i = 0; i < n; i++)
        A[n-i-1] = a[i+1] - '0', B[n-i-1] = b[i+1] - '0';
    for(int i = 0; i < 199; i++) // Add the two numbers together for 200 digits (overkill)
    {
        C[i] += A[i%n] + B[i%n];
        C[i+1] += C[i]/10;
        C[i] %= 10;
    }
    
    // Checks if C is periodic with period p beyond digit l
    auto isPeriodic = [] (int l, int p)
    {
        for(int i = l; i < l + 100; i++)
            if(C[i] != C[i+p])
                return false;
        return true;
    };

    int minp = std::numeric_limits<int>::max(), minl;
    for(int l = 0; l < 20; l++) // For each number of starting non-repeating digits
        for(int p = 1; p < 20; p++) // For each period length
            if(isPeriodic(l, p) && p < minp) // Check if this is a new minimum period
                minp = p, minl = l;
    std::cout << "(";
    for(int i = minp + minl - 1; i >= minl; i--)
        std::cout << char(C[i] + '0');
    std::cout << ")";
    for(int i = minl - 1; i >= 0; i--)
        std::cout << char(C[i] + '0');
}