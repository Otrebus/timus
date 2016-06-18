/* 1174. Weird Permutations - http://acm.timus.ru/problem.aspx?num=1174
 *
 * Strategy:
 * Looking at the output of the given algorithm, it generates numbers according to this rule:
 * For a set of numbers [1, n] to be permuted, all the permutations of [1, n-1] are generated in
 * order, and the number n is then "helixed" throughout all the permutations, so if we have
 * the permutations:
 * p1, p2, p3
 * p1, p3, p2     (2)
 *      ...
 * 
 * Then, adding the number n to this, we get:
 * 
 * p1, p2, p3, n
 * p1, p2, n, p3
 * p1, n, p2, p3
 * n, p1, p2, p3
 * n, p1, p3, p2   <-- note that we are now using the (2) permutation here
 * p1, n, p3, p2
 * p1, p3, n, p2
 * p1, p3, p2, p3
 *      ...
 *
 * This is done recursively. We can now formulate a recursive solution to the problem. If we have
 * the input sequence 1 4 3 2, we check in what position the permutation 1 3 2 occurs for the
 * subproblem for 3 digits, and then realize that for the problem of four digits, we have then moved
 * the number 4 through this permutation either two times or four times depending on whether the 4
 * was moving left or right at the time of the permutation. Determining the direction can be done
 * by checking the parity of the number of exchanges we have done; notice that during the first
 * permutation of the numbers 1 to 3, the 4 is moving leftwards, during the second permutation of
 * those numbers, the 4 is moving rightwards, etc.
 *
 * Performance:
 * An upper bound of the complexity is O(n^3) because of the bigint used; the program runs in
 * 0.001s using 284KB memory.
 */

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

class BigInt
{
    static const int base = 1000;
    static const int basewidth = 3;
public:
    std::vector<int> v;
    BigInt(int a = 0)
    {
        if(a)
            v.push_back(a);
    }

    BigInt operator+(BigInt b)
    {
        BigInt c;
        int carry = 0;
        v.resize(std::max(b.v.size(), v.size()), 0);
        b.v.resize(std::max(b.v.size(), v.size()), 0);
        for(int i = 0; i < v.size(); i++)
        {
            c.v.push_back((v[i]+b.v[i]+carry) % base);
            carry = (v[i]+b.v[i]+carry)/base;
        }
        if(carry)
            c.v.push_back(carry);
        return c;
    }

    BigInt operator*(int t)
    {
        BigInt c;
        int carry = 0;
        for(int i = 0; i < v.size(); i++)
        {
            c.v.push_back((v[i]*t+carry) % base);
            carry = (v[i]*t+carry)/base;
        }
        if(carry)
            c.v.push_back(carry);
        return c;
    }

    std::string str()
    {
        std::string ret;
        for(int i = v.size()-1; i >= 0; i--)
        {
            std::string s = std::to_string(v[i]);
            ret += (i < v.size()-1 ? std::string(basewidth-s.size(), '0') : "") + s;
        }
        return ret == "" ? "0" : ret;
    }
    friend BigInt operator*(int b, BigInt);
};

std::pair<BigInt, int> f(std::vector<int>& v, int n)
{
    if(n == 1)
        return { 0, 0 };
    int pre = 0;
    for(int i = 0; i < v.size() && v[i] != n; i++)
        if(v[i] < n)
            pre++;
    auto res = f(v, n-1);
    if(!res.second)
        return { res.first*n + (n-pre-1), (res.second*n + (n-pre-1)) % 2 };
    else
        return { res.first*n + pre, (res.second*n + pre) % 2 };
}


int main()
{
  int n, x;
  std::cin >> n;
  std::vector<int> v;
  for(int i = 0; i < n; i++)
  {
      std::cin >> x;
      v.push_back(x);
  }
  std::cout << (f(v, n).first + 1).str();
}