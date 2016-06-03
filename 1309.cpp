/* 1309. Dispute - http://acm.timus.ru/problem.aspx?num=1309
 *
 * Strategy:
 * Factorize g(x, y) into y(x^5-x+7) + x^3 + -x^5 + 3x, and notice that f(n) then becomes a simple
 * recursive function. If we precompute its values at discrete steps (here steps of 1e6), we can
 * terminate the recursion (or iteration) early enough to produce a result within the time limit.
 *
 * Performance:
 * O(n/sizeof(pre)), runs the test cases in 0.14s using 256KB memory.
 */

#include <iostream>

int pre[101] = { 0, 5392, 1890, 84, 6520, 3149, 2416, 2835, 80, 8614, 742, 7696, 6823, 9492, 7710,
                 9444, 510, 118, 6522, 3213, 4499, 6178, 4565, 763, 1071, 8875, 2688, 9145, 1211, 
                 9480, 4056, 1817, 8661, 5467, 3358, 2892, 2205, 8691, 1963, 2386, 8401, 1047, 3691,
                 6824, 825, 7728, 6797, 1720, 8194, 9901, 2823, 1952, 9344, 5022, 1421, 6116, 4511,
                 1289, 2133, 7494, 7298, 5012, 9638, 8753, 5968, 4029, 4804, 9556, 924, 1497, 5886,
                 6078, 2085, 3876, 268, 2910, 8962, 2970, 1015, 3931, 1103, 4872, 4054, 346, 1119,
                 931, 4454, 6530, 1722, 4266, 9888, 7961, 2891, 885, 4461, 7731, 3316, 2155, 93,
                 2871, 9710 };

int mod = 9973;

int modmin(int a, int b) // Modulo minus
{
    int res = a-b;
    return res < 0 ? mod + res : res % mod;
}

int modpow(int x, int n) // Modulo power
{
    if(n == 0)
        return 1;
    return (x*modpow(x, n-1))%mod;
}

int A(int x) // The factor of y in the expression of g(x, y) above
{
    x %= mod;
    return modmin(modpow(x, 5), x) + 7;
}

int B(int x) // The remaining term of the expression of g(x, y) above
{
    x %= mod;
    return (modmin(((3*x) + modpow(x,3))%mod, modpow(x, 5))) % mod;
}

int f(int n)
{
    int a = 1, total = 0;
    do
    {
        auto p = A(n);
        if(n == 0 || p == 0) // We can terminate early if A(n) turns out to be 0
            return total;
        if((n-1) % 1000000 == 0) // Here we can use the tabulated value
            return ((total + a*p%mod*pre[(n-1)/1000000])%mod+(a*B((n)))%mod)%mod;
        total = (total + a*B(n))%mod; // This is from making the recursive call into
        a = (p*a) % mod;              // an iterated expression
    } while(n--);
}

int main()
{
    int n;
    std::cin >> n;
    std::cout << f(n);
}