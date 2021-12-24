/* 1476. Lunar Code - http://acm.timus.ru/problem.aspx?num=1476
 *
 * Strategy:
 * Dynamic programming over A[n][k] which is the number of trusted matrices with n columns with
 * k zeroes in the last column. For each such a configuration of the last column, we can choose
 * to put a 0 in up to k' places when inserting a new column, which we can do either to the right
 * of a zero or a one in the previous column, which we can do in
 * 
 *  c[k][0]*c[M-k][k'] + c[k][1]*c[M-k][k'-1] + ...  C[k][k']*A[M-k][0]
 * 
 * ways (where c is the "n choose k" function) for valid combinations of k' as given in the
 * project description, mening at most K ones after a zero. To speed things up, we precalculate all
 * possible sums of c given above in a new matrix, denoted C.
 * 
 * Performance:
 * Something like O(n^4), runs the tests in 0.234 seconds using 828KB memory.
 */

#include <vector>
#include <string>

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

    BigInt operator+(const BigInt& b) {
        BigInt c;
        ll carry = 0;
        v.resize(std::max(b.v.size(), v.size()), 0);
        b.v.resize(std::max(b.v.size(), v.size()), 0);

        for(int i = 0; i < v.size(); i++) {
            auto cr = v[i]+b.v[i]+carry;
            auto res = div(cr, base);
            c.v.push_back(res.rem);
            carry = res.quot;
        }
        if(carry)
            c.v.push_back(carry);
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

BigInt c[41][41];
BigInt C[41][41];
BigInt A[40][41];
int M, N, K;

int main() {
    scanf("%d %d %d", &M, &N, &K);

    // Calculate the c(n, k) function
    c[0][0] = 1;
    for(int i = 1; i <= 40; i++)
        for(int j = 0; j <= i; j++)
            c[i][j] = (j == 0 || i == j) ? 1 : (c[i-1][j] + c[i-1][j-1]);

    // Initialize the first row of the matrix
    for(int i = 0; i < 41; i++)
        for(int j = 0; j < 40; j++)
            A[j][i] = 0;
    for(int i = 0; i <= M; i++)
        A[0][i] = c[M][i];

    // Calculate the "cumulative" C function
    for(int k1 = 0; k1 <= 40; k1++)
        for(int k2 = 0; k2 <= 40; k2++)
            for(int i = 0; i <= k1; i++)
                if(k1-i <= K && i <= k1 && k2-i <= M-k1)
                    C[k2][k1] += c[M-k1][k2-i]*c[k1][i];

    // Do the DP step
    for(int j = 1; j < N; j++) 
        for(int k = 0; k <= M; k++) // k = number of 0's we end up with
            for(int k1 = 0; k1 <= M; k1++) // k1 = number of 0's the original had
                A[j][k] += C[k][k1]*A[j-1][k1];

    BigInt ans = 0;
    for(int i = 0; i <= M; i++)
        ans = ans + A[N-1][i];
    printf("%s\n", ans.str().c_str());
}
