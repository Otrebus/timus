/* 1513. Lemon Tale - http://acm.timus.ru/problem.aspx?num=1513
 *
 * Strategy:
 * Let the answer for the case where we end the sequence of length i by a B be denoted B_i for some
 * given K. Then, we can calculate B_(i+1) by adding together all answers B_i down to B_(i-K), since
 * each of these is an answer set that we can tack a number of L's at the end of. See the below 
 * illustration for the case when K = 4:
 *
 * #########B =
 * ########B  +
 * #######BL  +
 * ######BLL  +
 * #####BLLL  +
 * ####BLLLL
 *
 * These added sequences of length i comprise all possible sequences of length i+1 that end with a 
 * B. To speed up the calculation, we realize that the solution for B_i also contains the sum down 
 * to B_(i-k-1), so the sum B_i + B_(i-1) + ... + B_(i-k) can be written 2*B_i - B_(i-k-1). Also, 
 * whenever i <= k we know that B_(i+1) = 2^i. Bignum is used for calculating the actual values; 
 * to speed up calculations, everything is in base 10^8.
 *
 * Performance:
 * O(N^2), running the tests in 0.031s (ranked 2/822) using 20044KB memory.
 */

#include <iostream>
#include <iomanip>

class BigInt
{
    int A[400]; // The digits
    int length;
    const int mod = 100000000; // Base
public:
    BigInt(int n) : length(0)
    {
        int a = 1;
        for(int i = 0; i < 10; i++)
        {
            length += bool(n);
            A[i] = n % mod;
            n /= 10;
        }
    }

    BigInt() : length(1)
    {
        A[0] = 1;
    }

    void operator= (const BigInt& b)
    {
        for(int i = 0; i < b.length; i++)
            A[i] = b.A[i];
        length = b.length;
    }

    void mul2() // Multiply by 2
    {
        int carry = 0;
        for(int i = 0; i < length; i++)
        {
            A[i] = A[i]*2 + carry;
            carry = A[i]/mod;
            A[i] %= mod;
        }
        if(carry)
            A[length++] = carry;
    }

    void minus(const BigInt& b) // We assume that the subtracted number is less than us
    {
        int carry = 0;
        for(int i = 0; i < length; i++)
        {
            A[i] -= (b.A[i]+carry);
            if(A[i] < 0)
            {
                A[i] += mod;
                carry = 1;
            }
            else
                carry = 0;
        }
        if(!A[length-1])
            length--;
    }
    friend std::ostream& operator<< (std::ostream& os, const BigInt& obj);
};

std::ostream& operator<< (std::ostream& os, const BigInt& b) 
{
    os << b.A[b.length-1];
    for(int i = b.length-2; i >= 0; i--)
        os << std::setw(8) << std::setfill('0') << b.A[i];
    return os;
}

BigInt b[10002];

int main()
{
    int n, k;
    std::cin >> n >> k;
    b[0] = 1;
    b[1] = 1;

    for(int i = 2; i <= k+1; i++)
    {
        b[i] = b[i-1];
        b[i].mul2();
    }
    for(int i = k+2; i <= n+1; i++)
    {
        b[i] = b[i-1];
        b[i].mul2();
        b[i].minus(b[i-k-2]);
    }
    std::cout << b[n+1] << std::endl;
}