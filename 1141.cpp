/* 1141. RSA Attack - http://acm.timus.ru/problem.aspx?num=1141
 *
 * Strategy:
 * Factorize n into p and q and apply the normal RSA decryption method.
 *
 * Performance:
 * K*sqrt(n) since factorizing is the most time-consuming part; runs the tests in 0.015s, 184KB.
 */

#include <stdio.h>
#include <cctype>

int ps[] = 
    { 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 
      83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173};

void gcd(int a, int b, int& d, int& x, int& y) // Extended euclidian algorithm
{
    if(b == 0)
        d = a, x = 1, y = 0;
    else
    {
        gcd(b, a % b, d, x, y);
        int xr = y;
        y = x - (a/b)*y;
        x = xr;
    }
}

int powmod(int x, int p, int n) // Modular exponentiation by squaring
{
    if(p == 0)
        return 1;
    if(p == 1)
        return x;
    if(p % 2 == 0)
        return powmod((x*x) % n, p/2, n);
    else
        return (x * powmod((x*x) % n, (p-1)/2, n)) % n;
}

inline int getint()
{
    int ret = 0, ch;
    while(!isdigit(ch = getc(stdin)));
    ret = ch - '0';
    while(isdigit(ch = getc(stdin)))
        ret = ret*10 + ch - '0';
    return ret;
}

int main()
{
    int K = getint();
    while(K--)
    {
        int e = getint(), n = getint(), c = getint(), i = -1, d, x, y;
        while(n % ps[++i]);
        int p = n/ps[i];
        int q = n/p;
        int np = (p-1)*(q-1);
        gcd(e, np, d, x, y); // Solve e*x = 1 mod np
        if(x < 0)
            x += np;
        int res = 1;
        printf("%d\n", powmod(c, x, n));
    }
}