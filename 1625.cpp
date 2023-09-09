/* 1625. Hankel Matrix - http://acm.timus.ru/problem.aspx?num=1625
 *
 * Strategy:
 * For a Hankel matrix for indices up to n, we calculate the next indices by expanding the matrix
 * with a new row and column and add new values α_{n+1} = 0, α_{n+2) = x. We can then use row
 * reduction to calculate the determinant of the new matrix, and set x to the appropriate value to
 * get the bottom right value equal to 1 (so we get the determinant of the full matrix equal to 1).
 * If we calculate the first few values of α this way we see that the odd alphas equal the Catalan
 * numbers.
 * 
 * Performance:
 * O(n^3), runs the tests in 0.015s using 268KB memory.
 */

#include <vector>
#include <string>
#include <iostream>

using ll = long long;

struct BigInt { // Standard BigInt stuff
    mutable std::vector<ll> v;
    static const ll base = 10000000;

    BigInt(ll a = 0) {
        if(a)
            v.push_back(a);
    }

    BigInt operator* (ll p) const {
        if(!p || p == 1)
            return p == 1 ? *this : BigInt(0);
        ll carry = 0;
        BigInt c;

        for(int i = 0; i < v.size(); i++) {
            c.v.push_back(v[i]*p + carry);
            auto res = div(c.v[i], base);
            carry = res.quot;
            c.v[i] = res.rem;
        }
        while(carry) {
            c.v.push_back(carry);
            carry = c.v.back()/base;
            c.v.back() %= base;
        }
        return c;
    }

    BigInt operator*(const BigInt& b) const {
        BigInt c;
        ll carry = 0;
        
        for(int i = 0; i < v.size(); i++) {
            BigInt t = b*v[i];
            t.v.insert(t.v.begin(), i, 0);
            c += t;
        }
        if(carry)
            c.v.push_back(carry);
        return c;
    }

    void operator+=(const BigInt& b) {
        ll carry = 0;
        v.resize(std::max(b.v.size(), v.size()), 0);
        b.v.resize(std::max(b.v.size(), v.size()), 0);
        for(int i = 0; i < v.size(); i++) {
            auto cr = v[i]+b.v[i]+carry;
            auto res = div(cr, base);
            carry = res.quot;
            v[i] = res.rem;
        }
        if(carry)
            v.push_back(carry);
    }

    std::string str() {
        std::string ret;
        for(int i = v.size()-1; i >= 0; i--) {
            std::string s = std::to_string(v[i]);
            ret += (i < v.size()-1 ? std::string(7-s.size(), '0') : "") + s;
        }
        return ret;
    }
};

BigInt A[101];
int n;

int main() {
    A[0] = A[1] = BigInt(1);
    scanf("%d", &n);

    for(int i = 2; i <= n; i++)
        for(int j = 0; j < i; j++)
            A[i] += A[j]*A[i-j-1];

    for(int i = 0; i < n*2-1; i++)
        std::cout << ((i % 2) ? "0" : A[i/2].str()) << std::endl;
}

// The program that calculates Hankel coefficients using row reduction

//  #include <vector>
//  #include <string>
//  #include <iostream>
//  
//  using ll = long long;
//  
//  #include <vector>
//  #include <string>
//  #include <iostream>
//  #include <cmath>
//  
//  class BigInt
//  {
//      static const long long base = 10;
//      static const int basewidth = 1;
//  public:
//      std::vector<int> v;
//      bool neg;
//      BigInt(bool b) : neg(b) {}
//      BigInt(int a = 0) : neg(false) {
//          if(a < 0) {
//              neg = true;
//              a = std::abs(a);
//          }
//          while(a) {
//              v.push_back(a%base);
//              a/=base;
//          }
//      }
//  
//      void trim() {
//          while(v.size() && v.back() == 0)
//              v.pop_back();
//          if(v.empty())
//              neg = false;
//      }
//  
//      BigInt abs() {
//          BigInt res = *this;
//          res.neg = false;
//          return res;
//      }
//  
//      BigInt operator-() const {
//          BigInt res = *this;
//          res.neg = !neg;
//          return res;
//      }
//  
//      BigInt operator+(BigInt b)
//      {
//          if(b.neg != neg)
//              return *this - (-b);
//          BigInt c(neg);
//          int carry = 0;
//          v.resize(std::max(b.v.size(), v.size()), 0);
//          b.v.resize(std::max(b.v.size(), v.size()), 0);
//          for(int i = 0; i < v.size(); i++)
//          {
//              c.v.push_back((v[i]+b.v[i]+carry) % base);
//              carry = (v[i]+b.v[i]+carry)/base;
//          }
//          if(carry)
//              c.v.push_back(carry);
//          c.trim();
//          return c;
//      }
//  
//      BigInt operator-(BigInt b)
//      {
//          if(b.neg != neg)
//              return *this + (-b);
//          if(this->abs() < b.abs())
//              return -(b - *this);
//          BigInt c(neg);
//          int carry = 0;
//          v.resize(std::max(b.v.size(), v.size()), 0);
//          b.v.resize(std::max(b.v.size(), v.size()), 0);
//          for(int i = 0; i < v.size(); i++)
//          {
//              int newcarry = 0;
//              if(v[i] < (b.v[i]+carry))
//                  newcarry = 1; 
//              c.v.push_back(((base+v[i])-(b.v[i]+carry)) % base);
//              carry = newcarry;
//          }
//          c.trim();
//          return c;
//      }
//  
//      BigInt operator*(BigInt b) {
//          BigInt res;
//          int n = v.size();
//          for(int i = 0; i < n; i++) {
//              BigInt c(neg);
//              c.v = std::vector<int>(i, 0);
//              int carry = 0;
//              v.resize(std::max(b.v.size(), v.size()), 0);
//              b.v.resize(std::max(b.v.size(), v.size()), 0);
//              for(int j = 0; j < b.v.size(); j++) {
//                  int res = v[i]*b.v[j]+carry;
//                  carry = res/base; 
//                  c.v.push_back(res%base);
//              }
//              if(carry)
//                  c.v.push_back(carry);
//              res = res + c;
//          }
//          res.neg = b.neg ^ neg;
//          res.trim();
//          return res;
//      }
//  
//      void operator+=(BigInt b) {
//          ll carry = 0;
//          v.resize(std::max(b.v.size(), v.size()), 0);
//          b.v.resize(std::max(b.v.size(), v.size()), 0);
//          for(int i = 0; i < v.size(); i++) {
//              auto cr = v[i]+b.v[i]+carry;
//              auto res = div(cr, base);
//              carry = res.quot;
//              v[i] = res.rem;
//          }
//          if(carry)
//              v.push_back(carry);
//      }
//  
//      bool operator<(BigInt b)
//      {
//          if(neg && b.neg)
//              return (-b < -*this);
//          if(neg ^ b.neg)
//              return neg;
//          while(v.size() && v.back() == 0)
//              v.pop_back();
//          while(b.v.size() && b.v.back() == 0)
//              b.v.pop_back();
//          if(v.size() != b.v.size())
//              return v.size() < b.v.size();
//          for(int i = v.size()-1; i >= 0; i--)
//          {
//              if(v[i] != b.v[i])
//                  return v[i] < b.v[i];
//          }
//          return false;
//      }
//  
//      bool operator<=(BigInt b) {
//          return !(b < *this);
//      }
//  
//      std::string str() {
//          std::string ret;
//          for(int i = v.size()-1; i >= 0; i--) {
//              std::string s = std::to_string(v[i]);
//              ret += (i < v.size()-1 ? std::string(basewidth-s.size(), '0') : "") + s;
//          }
//          return ret;
//      }
//  };
//  
//  BigInt A[201][201];
//  BigInt B[101];
//  int n;
//  
//  void reduce(int r, int n) {
//      for(int i = r+2; i < n; i += 2) {
//          auto p = A[i][r];
//          for(int j = r; j < n; j += 2) {
//              A[i][j] = A[i][j] - p*A[r][j];
//          }
//      }
//  }
//  
//  int main() {
//      B[0] = B[1] = BigInt(1);
//      scanf("%d", &n);
//  
//      for(int N = 3; N <= n; N++) {
//          for(int i = 0; i < 2*N-1; i++)
//              for(int j = 0; j < 2*N-1; j++)
//                  A[i][j] = 0;
//  
//          for(int i = 0; i < 2*N-1; i++)
//              for(int j = 0; j <= i; j++)
//                  A[i-j][j] = B[i/2];
//  
//          for(int i = 0; i < 2*N-1; i++) {
//              reduce(i, 2*N-1);
//          }
//          B[N-1] = BigInt(1) - A[N-1][N-1];
//      }
//  
//      for(int i = 0; i < n*2-1; i++) {
//          std::cout << ((i % 2) ? "0" : B[i/2].str()) << std::endl;
//      }
//  }
