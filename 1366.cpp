/* 1366. Presents - http://acm.timus.ru/problem.aspx?num=1366
 *
 * Strategy:
 * If we have children ABCDEFGH (in this notation the children always stand in order and the letter
 * indicates who the owner of the present is), we can let A get one of the other's presents. This
 * can be done in n-1 ways. Assume that H gave his present to A, so we get get HBCDEFG_ with A's 
 * present to be assigned to someone. If we give A to H, we then solve the middle BCDEFG which is
 * the problem of size n-2. On the other hand, if we do NOT give A's present to H, we have the 
 * problem where we need to assign A to anywhere but the _ in BCDEFG_, and the other presents to 
 * anywhere but their originator as always. This is equivalent to pretending that _ is actually A,
 * so this is in fact the problem size of n-1. We then get the recurrence relation
 * S(i) = (i-1)*(S(i-1)+S(i-2)) where S(i) is the solution of the problem for input i.
 *
 * Performance:
 * O(n^2) since we use bigint arithmetic and the answer grows by a constant number of digits per
 * iteration. Runs the test cases in 0.046s using 2664KB memory.
 */

#include <iostream>
#include <memory>
#include <vector>
#include <algorithm>
#include <string>

const int base = 1000;

std::vector<int> add(std::vector<int>& a, std::vector<int>& b) // Bigint addition
{
    std::vector<int> c;
    int carry = 0;
    a.resize(std::max(a.size(), b.size()), 0);
    b.resize(std::max(a.size(), b.size()), 0);
    for(int i = 0; i < a.size(); i++)
    {
        c.push_back((a[i]+b[i]+carry) % base);
        carry = (a[i]+b[i]+carry)/base;
    }
    if(carry)
        c.push_back(carry);
    return c;
}

std::vector<int> mul(std::vector<int>&& a, int b) // Bigint multiplication by normal int
{
    std::vector<int> c;
    int carry = 0;
    for(int i = 0; i < a.size(); i++)
    {
        c.push_back((a[i]*b+carry) % base);
        carry = (a[i]*b+carry)/base;
    }
    if(carry)
        c.push_back(carry);
    return c;
}

std::string toStr(std::vector<int>& a) // Bigint to string
{
    std::string ret;
    for(int i = a.size()-1; i >= 0; i--)
    {
        std::string s = std::to_string(a[i]);
        ret += (i < a.size()-1 ? std::string(3-s.size(), '0') : "") + s;
    }
    return ret;
}
std::string algo(int n) // As in the header comment
{
    std::vector<int> A[1001];
    A[0] = {1};
    A[1] = {0};
    for(int i = 2; i <= n; i++)
        A[i] = mul(add(A[i-2],A[i-1]), i-1);
    return toStr(A[n]);
}

int main()
{
    int n;
    std::cin >> n;
    std::cout << algo(n) << std::endl;
}