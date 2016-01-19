/* 2062. Ambitious Experiment - http://acm.timus.ru/problem.aspx?num=2062
 *
 * Strategy:
 * Store the cumulative updates in a Fenwick tree (range update, point query). When a point p is
 * queried in the input, query each divisor of p in the tree and sum together their values.
 *
 * Performance:
 * O(qdlog n), where d is the maximum number of divisors of any number n (here 180), runs the
 * tests in 1.887s using 43240KB memory.
 */

#include <stdio.h>
#include <cctype>
#include <vector>

const int maxn = 300000;
long long A[maxn+2]; // Fenwick tree array
int init[maxn+1]; // Initial values
int n;
std::vector<int> D[maxn+1]; // The divisors of each number

long long query(int i)
{
    long long sum = 0;
    while(i > 0) 
        sum += A[i], i -= i & -i;
    return sum;
}
 
void add(int i, long long k)
{
    while (i <= n) 
        A[i] += k, i += i & -i;
}

void update(int j, int k, long long v)
{
    add(j, v);
    add(k+1, -v);
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

void getDivs() // Fills D above
{
    for(int i = 1; i <= n; i++)
        for(int j = 1; j*i <= n; j++)
            D[j*i].push_back(i);
}

int main()
{
    n = getint();
    getDivs();
    for(int i = 1; i <= n; i++)
        init[i] = getint();
    int q = getint();
    while(q--)
    {
        int opt = getint();
        if(opt == 1)
        {
            int i = getint();
            long long ans = init[i];
            for(auto d : D[i])
                ans += query(d);
            printf("%lld\n", ans);
        }
        else
        {
            int l = getint(), r = getint(), d = getint();
            update(l, r, d);
        }
    }
}