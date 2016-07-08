/* 1932. The Secret of Identifier - http://acm.timus.ru/problem.aspx?num=1932
 *
 * Strategy:
 * Let |efxx| denote the number of pairs of integers that begin with the digits "ef" and where the
 * digits on positions x are unequal between every such pair. Then we can calculate:
 *
 *   |efxx| = |ef| - |ef?x| - |efx?|
 *
 * Where |ef| is the number of pairs of integers that begin with "ef" regardless if the pairs share
 * other digits and where |ef?x| denotes the number of pairs |ef0x| through |effx|, defined
 * similarly. We also have
 * 
 *   |exxx| = |e| - |e??x| - |ex??| - |e?x?| - |exx?| - |ex?x| - |e?xx|.
 *
 * The |efgx| numbers are straightforward to calculate (no exclusion/subtraction). All the |efxx|
 * numbers are stored in the variable C[16][65536] where the first index denotes the bitmask of the
 * fixed numbers (in the case efxx, this is 1100b) and the second index is the fixed integer
 * (for efxx, it's 0xef00). The |ef| numbers are stored in the M variable similarly.
 *
 * Performance:
 * In essence constant in n, but very exponential in the number of digits (in this problem 4).
 * The program runs in 0.015s and uses 2,408KB memory (rated 3/213).
 */

#include <stdio.h>

using uint = unsigned int;

unsigned short M[16][1 << 16];
uint C[16][1 << 16];
uint ans[5];
char bits[5][6]; // bits[i] contains all bitmasks that have i bits set
char p[5]; // p[i] = the number of bitmasks that have i bits set
char countbits[16]; // The reverse of the above
int maskmap[16]; // Maps 1101b to 0xFF0F, etc

// Combines num = 0xab and mask 0x0f0f into 0x0a0b, etc.
int fill(int num, int mask)
{
    int result = 0;
    for(int i = 0, pos = 0; i < 4; i++)
        if(mask >> i*4 & 0xF)
            result |= (num >> pos++*4 & 0xF) << i*4;
    return result;
}

// Given a position of fixed numbers in a set of pairs and the number of free 'x' digits, this
// method sums together all combinations thereof. For example, given fixed = 0xe, mi = 0100b,
// free = 2, iter = 1, this function returns |xex?| + |xe?x| + |?exx|.
uint fill(int fixed, int mi, int free, int iter)
{
    uint res = 0;
    int base = fill(fixed, maskmap[mi]);
    for(int i = 0; i < p[free]; i++)
    {
        int m = bits[free][i], m2 = m | mi;
        if(countbits[m2] != 4-iter)
            continue;
        for(int bound = 0; bound < 1 << free*4; bound++)
            res += C[m2][base | fill(bound, maskmap[m])];
    }
    return res;
}

// Calculates the equations given in the header comment, for every combination; n is the number
// of bound digits and the T function is what we subtract in each equation
template<typename T> uint calc(int n, T fun)
{
    uint res = 0;
    for(int i = 0, m = bits[n][i]; i < p[n]; m = bits[n][++i])
    {
        for(int i = 0; i < 1 << n*4; i++)
        {
            int s = fill(i, maskmap[m]);
            uint a = M[m][maskmap[m]&s], b = fun(i, m);
            res += C[m][maskmap[m]&s] = a*(a-1)/2 - b;
        }
    }
    return res;
}

int main()
{
    uint N, x;
    scanf("%u", &N);

    for(char m = 0; m < 16; m++)
    {
        int num = 0; char n = 0;
        for(int j = 0; j < 4; j++)
            if((m >> j) & 1)
                num |= (0xf << 4*j), n++;
        maskmap[m] = num;
        bits[n][p[n]++] = m;
        countbits[m] = n;
    }

    for(uint i = 0; i < N; i++)
    {
        scanf("%x", &x);
        for(int m = 0; m < 16; m++)
            M[m][maskmap[m] & x]++;
    }

    uint a3 = calc(3, [] (int, int) { return 0; });
    uint a2 = calc(2, [] (int i, int m) { return fill(i, m, 1, 1); });
    uint a1 = calc(1, [] (int i, int m) { return fill(i, m, 2, 1) + fill(i, m, 1, 2); });
    uint a0 = N*(N-1)/2 - a1 - a2 - a3;
    printf("%u %u %u %u", a3, a2, a1, a0);
}