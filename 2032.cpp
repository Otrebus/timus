/* 2032. Conspiracy Theory and Rebranding - http://acm.timus.ru/problem.aspx?num=2032
 *
 * Strategy:
 * Naive strategy. Generate vectors that are as long as the first two input numbers, and then,
 * starting at position (0, 0), go along the vectors generated from the first number, then for
 * each such vector, along the vectors of the second number, and then check if the length of the
 * resulting vector back to the origin equals the third input number.
 *
 * Performance:
 * O(a + b), assuming that there are less than a quadratic amount of triplets for any given
 * integer. Runs the test cases in 0.187s using 396KB memory.
 */

#include <iostream>
#include <algorithm>
#include <vector>

// Generates all pythagorean triples that have the third number equal to c
std::vector<std::pair<long long, long long>> triples(long long c)
{
    std::vector<std::pair<long long, long long>> v;
    long long a = 0, b = c;
    while(b >= a)
    {
        while(a*a + b*b > c*c)
            b--;
        if(a*a + b*b == c*c && a <= b)
            v.push_back( { std::min(a, b), std::max(a, b) } );
        a++;
    }
    return v;
}

// Returns all vectors that are as long as the input vector with the absolute values of the
// components retained or switched
std::vector<std::pair<long long, long long>> perms(std::pair<long long, long long> in)
{
    std::vector<std::pair<long long, long long>> v;
    for(int i = -1; i <= 1; i += 2)
        for(int j = -1; j <= 1; j += 2)
            v.push_back( { i*in.first, j*in.second } ), v.push_back( { i*in.second, j*in.first } );
    return v;
}

int main()
{
    long long A, B, C;
    std::cin >> A >> B >> C;
    auto a = triples(A);
    auto b = triples(B);
    auto c = triples(C);

    for(int i = 0; i < a.size(); i++)
    {
        auto p1 = a[i];
        for(int j = 0; j < b.size(); j++)
        {
            auto bp = perms(b[j]);
            for(int k = 0; k < bp.size(); k++)
            {
                auto p2 = bp[k];
                auto x = p1.first + p2.first;
                auto y = p1.second + p2.second;
                if(x*x + y*y == C*C)
                {
                    std::cout << "0 0\n" 
                                << p1.first << " " << p1.second << "\n" 
                                << x << " " << y << "\n";
                    return 0;
                }
            }
        }
    }
    std::cout << "-1\n";
    return 0;
}