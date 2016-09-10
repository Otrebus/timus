/* 1974. Similar Tunes - http://acm.timus.ru/problem.aspx?num=1974
 *
 * Strategy:
 * Naive.
 *
 * Performance:
 * O(mn^2), where n is the maximum tune length, runs the tests in 0.187s using 316KB memory.
 */

#include <iomanip>
#include <algorithm>
#include <string>
#include <iostream>
#include <unordered_map>

char A[1000], B[1000];

std::unordered_map<std::string, int> map = 
    { { "C", 0 }, { "C+", 1 }, { "D", 2 }, { "D+", 3 }, { "E", 4 }, { "F", 5 }, 
      { "F+", 6 }, { "G", 7 }, { "G+", 8 }, { "A", 9 }, { "A+", 10 }, { "B", 11 } };

int read(char* c)
{
    int n, o;
    std::cin >> n;
    for(int i = 0; i < n; i++)
    {
        std::string s;
        std::cin >> o >> s;
        auto it = map.find(s);
        if(it != map.end()) // Standard notation
            c[i] = o*12 + it->second;
        else // Alternative notation
        {
            auto t = s.back() == '+' ? 1 : -1;
            s.pop_back();
            c[i] = o*12 + map[s] + t;
        }
    }
    return n;
}

int compare(int alen, int blen)
{
    int max = 0;
    // For every way to offset string b against string a ..
    for(int offset = -blen + 1; offset < alen; offset++)
    {
        int c = 0;
        int L = std::max(offset, 0), R = std::min( { blen + offset, alen } );
        // .. calculate the number of matching characters
        for(int i = L; i < R; i++) 
            c += A[i] == B[i-offset];
        max = std::max(c, max);
    }
    return max;
}

int main()
{
    std::ios::sync_with_stdio(false);
    int a = read(A);
    int m;
    std::cin >> m;
    while(m--)
    {
        double k = read(B);
        double d = compare(a, k);
        std::cout << std::setprecision(20) << (d/k) << "\n";
    }
}