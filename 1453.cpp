/* 1453. Queen - http://acm.timus.ru/problem.aspx?num=1453
 *
 * Strategy:
 * Find the shortest direction (with length d) that the queen can move in, meaning the side of
 * the chessboard closest to the queen, and cut the search space parallel to that side into two
 * halfspaces centered at the queen. The queen can then move d units in every direction that
 * involves the smaller halfspace, which is d*2^N units in total. We can then recursively apply
 * this in the opposite halfspace with the remaining N-1 dimensions. In the code below we do this
 * procedure the other way around - we start in 1d with the dimension with the biggest side (the
 * queen closest to the center), add the squares we can make in the smaller dimension, then expand
 * into the next dimension (the second least unbalanced one from the point of the queen) add the
 * number of units we can move involving the smallest direction of those dimensions, etc.
 *
 * Performance:
 * O(N^2*log S), runs the tests in 0.125s using 1,328KB memory.
 */

#include <stdio.h>
#include <algorithm>
#include <vector>
#include <sstream>
#include <string>

int N, S, i;
int C[100000];

const int base = 10000;
struct BigInt { // Standard bigint class, nothing to see
    std::vector<int> v;

    BigInt(int a = 0) {
        if(a)
            v.push_back(a);
    }

    BigInt operator*(int t) {
        BigInt c;
        int carry = 0;
        for(int i = 0; i < v.size(); i++) {
            c.v.push_back((v[i]*t+carry) % base);
            carry = (v[i]*t+carry)/base;
        }
        if(carry)
            c.v.push_back(carry);
        return c;
    }
    BigInt operator*= (int p) { return *this = *this*p; }

    BigInt operator+(BigInt b) {
        BigInt c;
        int carry = 0;
        v.resize(std::max(b.v.size(), v.size()), 0);
        b.v.resize(std::max(b.v.size(), v.size()), 0);
        for(int i = 0; i < v.size(); i++) {
            c.v.push_back((v[i]+b.v[i]+carry) % base);
            carry = (v[i]+b.v[i]+carry)/base;
        }
        if(carry)
            c.v.push_back(carry);
        return c;
    }
    BigInt operator+= (BigInt b) { return *this = *this + b; }

    std::string toString() {
        std::stringstream ss;
        int l = 0;
        for(int b = base/10; b; b/=10)
            l++;
        int i = v.size()-1;
        while(!v[i] && i > 0)
            i--;
        ss << std::to_string(v[i--]);
        for(; i >= 0; i--) {
            ss.width(l), ss.fill('0');
            ss << std::to_string(v[i]);
        }
        return ss.str();
    }
};

// The the smaller and bigger distance to the edge of the chessboard along some dimension
int sm(int a) { return std::min(std::abs(S-a), std::abs(a-1)); }
int bg(int a) { return std::max(std::abs(S-a), std::abs(a-1)); }

int main() {
    for(scanf("%d %d", &N, &S); i < N; i++)
        scanf("%d", &C[i]);

    if(N == 1) // One dimension is a special case
        return printf("%d", S-1), 0;

    // Sort the dimensions by the shortest distance to a side, reversed
    std::sort(C, C+N, [] (int& a, int& b) { return sm(a) > sm(b); });

    BigInt b(1); // The 2^i factor
    // The number of rook moves is simple, just (S-1)*N
    BigInt ans(bg(C[0]) + (S-1)*(N)); // Also, we need to account for the very first biggest dir

    // Progressively expand the movement space along each new dimension, adding 2^i*d units
    // in the smaller direction (distance d to the closest side)
    for(int i = 0; i < N; i++)
        ans += (i >= 1 ? b *= 2 : b)*sm(C[i]);

    printf("%s", ans.toString().c_str());
}
