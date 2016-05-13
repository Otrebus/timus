/* 1172. Ship Routes - http://acm.timus.ru/problem.aspx?num=1172
 *
 * Strategy:
 * Let A[i, j, k] represent the number of ways that we can travel between the islands, starting at
 * the first island (with i _remaining_ ports) and having j and k remaining ports on the other two
 * islands, but without the stated requirement that we do not end up on the first island (if we
 * ended up on the first island we would have to travel by land back to the initial point on the
 * first island). We can travel to j different ports on the second island, and k different ports on
 * the third island, and then treat each of those islands as our first island with j-1 and k-1
 * remaining ports to visit, giving the equation:
 * 
 *   A[i, j, k] = j*A[j-1, i, k] + k*A[k-1, i, j].
 * 
 * Now let B[i, j, k] be the number of ways we can travel between the islands but where we visit 
 * the last (unique) port at islands j or k. This is the number of valid paths requested by the
 * problem.
 *   Let's try to get a recurrence relation for B. If a valid path for the case of 3 ports on the 
 * first island looks like this (the "..." are paths involving j and k ports only):
 * 
 *  ... i_a ... i_b ... i_c ... 
 *
 * , where a, b, and c are unique indices in [1,3], then from each such route we can make four
 * different longer invalid paths for the problem instance of (i+1, j, k) ports like this:
 *
 * ... i_4 ... i_a ... i_b ... i_c,
 * ... i_a ... i_4 ... i_b ... i_c, 
 *
 * , and so on. This gives rise to the relation:
 *
 *   A[i+1, j, k] - B[i+1, j, k] = B[i, j, k]*(i+1)
 *
 * Or (if we translate i+1 into i) the difference between the number of invalid and valid paths for
 * the case of i ports on the first island equals i times the amount of valid paths for i-1 ports 
 * on the first island. We can now plot down these functions in code.
 *   Since the numbers involved grow greater than what 64 bits can handle, we use a custom big
 * integer class, and we also memoize the values in the recursive functions.
 *
 * Performance:
 * Omega(N^3) (because of BigInt), runs the test cases in 0.031s using 1,416KB memory.
 */

#include <iostream>
#include <algorithm>
#include <vector>
#include <string>

class BigInt // Class representing large numbers (somewhat unoptimized implementation)
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

    BigInt operator-(BigInt b)
    {
        BigInt c;
        int carry = 0;
        v.resize(std::max(b.v.size(), v.size()), 0);
        b.v.resize(std::max(b.v.size(), v.size()), 0);
        for(int i = 0; i < v.size(); i++)
        {
            int newcarry = 0;
            if(v[i] < (b.v[i]+carry))
                newcarry = 1; 
            c.v.push_back(((base+v[i])-(b.v[i]+carry)) % base);
            carry = newcarry;
        }
        if(c.v.back() == 0)
            c.v.pop_back();
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

    BigInt operator/(int t)
    {
        BigInt c;
        int carry = 0;
        for(int i = v.size()-1; i >= 0; i--)
        {
            int res = (carry+v[i])/t;
            carry = ((carry+v[i])-res*t)*base;
            c.v.push_back(res % base);
        }
        std::reverse(c.v.begin(), c.v.end());
        if(c.v.back() == 0)
            c.v.pop_back();
        return c;
    }

    bool operator==(BigInt b)
    {
        return !(v < b.v) && !(b.v < v);
    }

    bool operator<(BigInt b)
    {
        if(v.size() != b.v.size())
            return v.size() < b.v.size();
        for(int i = v.size()-1; i >= 0; i--)
        {
            if(v[i] != b.v[i])
                return v[i] < b.v[i];
        }
        return false;
    }

    std::string str()
    {
        std::string ret;
        for(int i = v.size()-1; i >= 0; i--)
        {
            std::string s = std::to_string(v[i]);
            ret += (i < v.size()-1 ? std::string(basewidth-s.size(), '0') : "") + s;
        }
        return ret;
    }
    friend BigInt operator*(int b, BigInt);
};

BigInt operator*(int t, BigInt b)
{
    BigInt c;
    int carry = 0;
    for(int i = 0; i < b.v.size(); i++)
    {
        c.v.push_back((b.v[i]*t+carry) % BigInt::base);
        carry = (b.v[i]*t+carry)/BigInt::base;
    }
    if(carry)
        c.v.push_back(carry);
    return c;
}

BigInt C[31][31][31];
bool calced[31][31][31]; // Calculated already?

BigInt A(int i, int j, int k)
{
    if(i == 0 && j == 0 && k == 0)
        return 1;
    else if(i < 0)
        return 0;
    else
    {
        if(!calced[i][j][k])
        {
            // Here we optimize slightly by always having min(j, k) as the second argument
            C[i][j][k] = j*A(j-1, std::min(i, k), std::max(i, k)) 
                       + k*A(k-1, std::min(i, j), std::max(i, j));
            calced[i][j][k] = true;
        }
        return C[i][j][k];
    }
}

BigInt B(int i, int j, int k)
{
    if(i < 0)
        return 0;
    else
        return A(i, std::min(j, k), std::max(j, k)) - (i*B(i-1, j, k));
}

int main()
{
    int N;
    std::cin >> N;
    std::cout << (B(N-1, N, N)/2).str() << std::endl;
}