/* 1217. Unlucky Tickets - http://acm.timus.ru/problem.aspx?num=1217
 *
 * Strategy:
 * Dynamic programming. For each number with i digits (i not necessarily even), define the 'digit 
 * difference' to be the difference between the sums of the even and odd digits (even and odd 
 * referring to their position) and the digit sum to be the sum of all digits. For example, the
 * digit difference of a four digit  number can be between -18 and 18. For each number length, each
 * possible digit difference in this length and each possible sum in this digit difference, we store
 * the number of different digits satisfying this constraint. For example, 3070 and 2080 are two 
 * different numbers with the same digit difference and digit sum. The number of such numbers is 
 * stored as t[12][10] below. Such an array is built for numbers of length i by using the array for
 * digits of length i-1. Finally, the array for length N/2 is used to calculate the number of 
 * numbers requested.
 *
 * Performance:
 * O(N^3), running the tests in 0.015s using 512KB memory.
 */

#include <stdio.h>
#include <algorithm>
#include <cstring>

long long T1[201][101];
long long T2[201][101];

int main()
{
    int N;
    scanf("%d", &N);
    N /= 2;

    long long (*s)[101] = T1+100, (*t)[101] = T2+100; // So that we can index by negative numbers

    for(int i = 0; i <= 9; i++) // Populate the base case of one digit
        s[-i][i] = 1; // 1 is an odd number of digits, so the digit difference is negative
    for(int n = 2; n <= N; n++)
    {
        std::swap(s, t); // t is the array for numbers of length n-1, and we write to s
        std::memset(s-100, 0, sizeof(T1));
        for(int digit = 0; digit <= 9; digit++) // We append one digit, from 0 to 9
        {
            // One example of one iteration of the below: all numbers with digit difference 5 and 
            // digit sum 11, like 5132, when getting the digit 2 appended to them, become numbers
            // one digit longer, like 25132, with digit difference 3 and digit sum 13
            if(n % 2 == 1)
                for(int dif = -9*(n/2); dif <= 9*(n/2); dif++)
                    for(int sum = 0; sum <= 9*n; sum++)
                        s[dif-digit][sum+digit] += t[dif][sum];
            else
                for(int dif = -9*(1+n/2); dif <= 9*(n/2); dif++)
                    for(int sum = 0; sum <= 9*n; sum++)
                        s[dif+digit][sum+digit] += t[dif][sum];
        }
    }

    long long ans = 0;
    // Depending on if N/2 is even or odd, we piece together digits of either equal or opposing
    // digit differences
    for(int dif = -9*(1+N/2); dif <= 9*(N/2); dif++)
        for(int sum = 0; sum <= N*9; sum++)
            ans += s[dif][sum]*s[(N % 2 ? 1 : -1)*dif][sum];

    printf("%lld", ans);
}