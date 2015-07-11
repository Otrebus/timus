/* 1648. Yachts - http://acm.timus.ru/problem.aspx?num=1648
 *
 * Strategy:
 * First, assume that the factory is producing at full capacity throughout the period and calculate
 * the amount of yachts sold given the rules, which is the first answer. With this strategy, there
 * will be an amount of yachts in storage at the end of the period, which serve no purpose but
 * increase the storage cost, so "continuously" remove one yacht/month worth of production from as
 * early as possible in the period (as early as possible is always the first month with 0 yachts in
 * storage as we keep removing production).
 *
 * Performance:
 * O(n) using 560KB of memory and runs the test cases at 0.001s which seems to be the fastest
 * solution to date.
 */

#include <stdio.h>
#include <vector>

int main()
{
    int n, d;
    scanf("%d %d", &n, &d);
    std::vector<int> A(n); // Purchasers of that month
    std::vector<long long> S(n); // Total yachts in storage that month
    std::vector<int> Z(n); // The smallest amount of yachts in storage after this month
    long long stored = 0, sold = 0, cost = 0; // Total amount stored, sold, and storage cost

    // Read all data and calculate sellings and the amount of yachts in storage if we
    // keep the production at max
    for(int i = 0; i < n; i++)
    {
        int a;
        scanf("%d", &a);
        A[i] = a;
        stored += d; // Max production
        long long cursold = std::min(stored, (long long) a); // How many we can sell this month
        sold += cursold;
        stored -= cursold;
        cost += stored;
        S[i] = stored;
    }
    // Calculate the first month in the last [r..n-1] period of nonzero yachts in storage
    int r = n-1;
    for(int i = n-1; i >= 0; i--)
    {
        if(S[i] == 0)
            break;
        r = i;
    }
    // Z[i] points to the index in S of the minimum amount of yachts in storage after i
    int m = n-1;
    for(int i = n-1; i >= r; i--)
    {
        if(S[i] < S[m])
            m = i;
        Z[i] = m;
    }
    long long totred = 0; // If we remove one yacht/month production at some point i, all months
                          // after that we will have one yacht less in storage - totred reflects
                          // what to subtract the S[i] values with to get the real amount stored
    // Keep removing production from as early as possible until we have 0 amount stored at the
    // end of the period
    for(int i = r; i < n && S[n-1]-totred > 0; )
    {
        long long red = std::min(S[n-1]-totred, S[Z[i]]-totred);
        cost -= (n-i)*red; // We won't have to store this yacht for the rest of the period
        totred += red;
        if(Z[i] == i)
            i++;
        else
            i = Z[i]; // Jump to the month further ahead with the smallest amount of yachts stored
    }
    printf("%lld %lld", sold, cost);
    return 0;
}
