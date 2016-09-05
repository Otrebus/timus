/* 2092. Bolero - http://acm.timus.ru/problem.aspx?num=2092
 *
 * Strategy:
 * For every possible subscription discount (1%-100%) and for every venue, calculate the amount of
 * money that can be saved by choosing to pay for that venue with the subscription plan instead of
 * the student discount. For the discount that matches some subscription plan (p, k), we then
 * sort all such savings, and choose the k or more largest savings - as long as they are positive -
 * and match those with the subscription plan and use the student credit for the rest. To speed
 * things up a bit, radix sort is used to sort the numbers.
 *
 * Performance:
 * O(n), runs the tests in 0.327s using 3,356KB memory.
 */

#include <algorithm>
#include <stdio.h>
#include <cstring>

const int maxn = 100000;

std::pair<int, int> subs[maxn];
int s[maxn], d[maxn];
int A[maxn], B[maxn]; // For radix sort
long long C[maxn]; // Cumulative savings
int h[1<<11]; // Radix sort histogram
const int mask = (1 << 11) - 1;

// Radix sort
void sort(int n)
{
    unsigned int* a = (unsigned int*) A, *b = (unsigned int*) B;
    for(int i = 0; i < n; i++) // Map [-2147483647, 2147483648] -> [0, 4294967295]
        a[i] += 0x80000000;
    for(int k = 0; k < 3; k++)
    {
        std::memset(h, 0, sizeof(h));
        for(int i = 0; i < n; i++) // Populate the buckets
            h[(a[i]>>(11*k)) & mask]++;    
        int s = 0;
        for(int i = (1 << 11) - 1; i >= 0 ; i--) // Make buckets into histograms
        {
            int tmp = h[i];
            h[i] = s;
            s += tmp;
        }
        for(int i = 0; i < n; i++) // Populate the array according to the histograms
            b[h[(a[i]>>(k*11)) & mask]++] = a[i];

        std::swap(a, b);
    }
    for(int i = 0; i < n; i++) // Map back
        b[i] = a[i] - 0x80000000;
}

int main()
{
    int n, m;
    scanf("%d %d", &n, &m);
    long long tot = 0; // Cost if we use student rebates only
    for(int i = 0; i < n; i++)
    {
        scanf("%d %d", &s[i], &d[i]);
        tot += s[i]*(100-d[i]);
    }
    for(int i = 0; i < m; i++)
        scanf("%d %d", &subs[i].second, &subs[i].first);
    std::sort(subs, subs + m);
    long long min = tot;
    int p = -1; // Current subscription percentage
    int P = 0;  // Position of the last positive saving
    for(int c = 0; c < m; c++)
    {
        if(subs[c].first != p)
        {
            p = subs[c].first;
            for(int i = 0; i < n; i++)
                A[i] = s[i]*(p - d[i]); // What we save by subscribing
            sort(n);

            // Calculate cumulative savings
            C[0] = A[0];
            for(int j = 1; j < n; j++)
            {
                if(A[j] >= 0)
                    P = j;
                C[j] = C[j-1] + A[j];
            }
        }

        // Calculate the optimum cost; only subscribe as long as we save a positive amount
        int k = subs[c].second;
        min = std::min(min, tot - C[std::max(k-1, P)]);
    }
    printf("%.2lf", min/100.0);
}