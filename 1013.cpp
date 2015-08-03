/* 1013. K-based Numbers. Version 3 - http://acm.timus.ru/problem.aspx?num=1013
 *
 * Strategy:
 * First, multiplication of large numbers modulo M uses the usual laws (a mod M + b mod M) = 
 * (a + b) mod M and similar for multiplication, to repeatedly shave off the largest power 
 * of 2 from a and adding that power multiplied by b to the answer. To solve the problem, we 
 * keep track of two types of subsolutions: the K-based numbers of length L that end with 0, 
 * and the number of K-based numbers of lengths L that end with any other digit (in the code 
 * denoted s0 and sk respectively). These subsolutions are multiplied together into the final 
 * answer in the same manner as the integers, described above. Given subsolutions a and b, in
 * brackets, of sizes A and B, the configurations below represent all the possible K-based
 * numbers of size A + B + 1:
 *
 *  [k  a  0/k][k  b  0]  k
 *  [k  a  0/k][k  b  k] 0/k
 *  [k  a   k ] 0 [k  b  0/k]
 *
 * The recurrence relation is in the general case:
 *  c_0 = (a_0 + a_k)*b_k + a_k*b_0,
 *  c_k = (a_0 + a_k)*b_0*(k-1) + (a_0 + a_k)*b_k*(k-1) + a_k*b_k.
 *
 * Performance:
 * With two nested logarithmic loops, the algorithm has time complexity O(log^2 n), which in 
 * practice runs in 0.015s using 216KB of memory.
 */

#include <stdio.h>
#include <algorithm>

inline unsigned long long mul(unsigned long long a, unsigned long long b, unsigned long long m)
{
    unsigned long long add = 0;
    if(a == 0 || b == 0)
        return 0;
    while(a > 1)
    {
        add = (add + a%2*b)%m;
        b = (b*2)%m;
        a /= 2;
    }
    return (b + add)%m;
}

struct Solution
{
    unsigned long long s0, sk, k, m, size;

    Solution(unsigned long long k, unsigned long long m) : sk(0), s0(0), k(k), m(m), size(1)
    {
    }

    void multiply(Solution b)
    {
        // First we have to take care of the special cases not covered by the recurrence
        // relation, since for example solutions of length 1 can't end in k and 0 at the
        // same time
        if(b.size + size == 2)
            sk = (k-1)%m;
        else if(b.size < size) // Assuming the argument is greater cuts down on special cases
        {
            b.multiply(*this);
            *this = b;
        }
        else if(size == 1) 
        {
            sk = mul(b.s0+b.sk, (k-1)%m, m);
            s0 = b.sk;
        }
        else // The recurrence relation in its general form
        {
            unsigned long long mys0 = (mul((s0+sk)%m, b.sk, m) + mul(sk, b.s0, m))%m;
            sk = (mul((s0+sk)%m, mul((b.s0+b.sk)%m, (k-1)%m, m), m) + mul(sk, b.sk, m))%m;
            s0 = mys0;
        }
        size = b.size + size;
    }
};

int main()
{
    unsigned long long N, K, M;
    scanf("%llu %llu %llu", &N, &K, &M);
    unsigned long long n = N;
    Solution ans(K, M);
    while(N > 0)
    {
        Solution s(K, M);
        while(2*s.size <= N)
            s.multiply(s);
        ans.multiply(s);
        N -= s.size;
    }

    printf("%llu\n", (ans.sk+ans.s0)%M);
    return 0;
}