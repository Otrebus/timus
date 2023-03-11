/* 1310. ACM Diagnostics - http://acm.timus.ru/problem.aspx?num=1310
 *
 * Strategy:
 * Dynamic programming over A[l][k] where A is the number of states, l the number of registers and
 * k the total sum outside those registers. For some l and some sought state n we can then sum
 * together the number of states A[l-1][k'] for each number 1..M of register l (where k' depends
 * on each value of register l), and stop and recurse, shaving off n, once the sum exceeds the
 * state number n. We then memoize the number we calculated. Since the numbers involved are quite
 * big we use a BigInt class.
 *
 * Performance:
 * Runs the tests in 0.156s using 2,472KB memory.
 */

#include <iostream>
#include <vector>
#include <array>

struct BigInt { // Bigint stuff, nothing to see here
    std::vector<int> num;

    BigInt(const std::string& str) {
        for(int i = str.size() - 1; i >= 0; i--)
            num.push_back(str[i]-'0');
        clean();
    }

    BigInt(int a = 0) {
        if(a)
            num.push_back(a);
    }

    void clean() {
        while(num.size() && !num.back())
            num.pop_back();
    }

    bool operator< (BigInt b) const {
        auto a = *this;
        a.clean(), b.clean();
        if(a.num.size() != b.num.size())
            return a.num.size() < b.num.size();
        std::reverse(a.num.begin(), a.num.end()), std::reverse(b.num.begin(), b.num.end());
        return a.num < b.num;
    }

    bool operator >= (BigInt b) const {
        return !(*this < b);
    }

    void operator+=(BigInt b) {
        int carry = 0;
        num.resize(std::max(b.num.size(), num.size()), 0);
        b.num.resize((int)std::max(b.num.size(), num.size()), 0);
        for(int i = 0; i < num.size(); i++) {
            auto cr = num[i]+b.num[i]+carry;
            auto res = div(cr, 10);
            carry = res.quot;
            num[i] = res.rem;
        }
        if(carry)
            num.push_back(carry);
    }

    BigInt operator-(BigInt b) {
        BigInt c;
        int carry = 0;
        num.resize(std::max(b.num.size(), num.size()), 0);
        b.num.resize(std::max(b.num.size(), num.size()), 0);
        for(int i = 0; i < num.size(); i++) {
            int newcarry = num[i] < (b.num[i]+carry);
            c.num.push_back(((10+num[i])-(b.num[i]+carry)) % 10);
            carry = newcarry;
        }
        if(c.num.back() == 0)
            c.num.pop_back();
        return c;
    }

    int toInt() {
        int ret = 0;
        for(int i = num.size()-1; i >= 0; i--)
            ret = ret*10 + num[i];
        return ret;
    }
};

BigInt A[101][51]; // Memoized A
bool B[101][51]; // Is this position memoized?
int R[101]; // The registers as we calculate them

int K, L, M;

BigInt calc(int l, int k) { // Calculates A[l][k]
    if(B[l][k]) // Already memoized
        return A[l][k];

    if(l == 1) // Down to one register
        return (M+k)/K;

    BigInt sum = BigInt(0);
    for(int i = 1; i <= M; i++) // Recurse
        sum += calc(l-1, (k+i)%K);

    B[l][k] = true;
    A[l][k] = sum;
    return sum;
}

void find(int l, int k, BigInt N) { // Finds the registers that amount to state N
    if(l == 1) {
        R[l] = std::max(1, K*N.toInt()-k);
        return;
    }

    // Sum states over the A[l-1][k]'s until we meet or exceed our state goal, then recurse
    BigInt sum = 0;
    for(int i = 1; i <= M; i++) {
        auto pre = sum;
        sum += calc(l-1, (i+k)%K);
        if(sum >= N) {
            find(l-1, (i+k)%K, N-pre);
            R[l] = i; // Save the register content
            return;
        }
    }
}

int main() {
    std::string N;
    std::cin >> L >> M >> K >> N;
    BigInt n(N);
    n += BigInt(1);

    find(L, 0, n);

    for(int i = L; i >= 1; i--)
        std::cout << R[i] << " ";
}
