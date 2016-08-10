/* 1830. Help in the RNOS - http://acm.timus.ru/problem.aspx?num=1830
 *
 * Strategy:
 * We scan the strings from right to left, and whenever we meet an unmatched digit, we set the digit
 * to the left of this as 1 and all the previous ones to zero, so that we are "on" that digit and
 * can change the next one (the unmatched digit).
 * The procedure to set a digit i to 1 (or 0) and all the left ones to zero, here called "set", can
 * be defined recursively. First "set" the digit i-1 to one preceded by zeroes, flip digit i and
 * then "set" the digit i-1 back to zero. If set is called on a string that already consists of all
 * zeroes preceding the digit to set, which is the case of the second invocation of "set" above, we
 * note that we can use a simple recurrence:
 *
 *                        j
 *  initial value  00000001
 *                        
 *    set(j, 0)    00000000
 *        =    
 *   set(j-1, 1)   00000011
 *        +
 *        1        00000010
 *        +
 *   set(j-1, 0),  00000000
 *
 * The second argument, the value to set, does not affect the recursion, so the recurrence
 * set(j, 1) = set(j-1, 1) + set(j-1, 0) + 1 reduces to set(j, 1) = 2^j - 1.
 *
 * Performance:
 * O(n^2), runs the tests in 0.015s using 172KB memory.
 */

#include <stdio.h>

int N;
unsigned long long ans;
char cur[52];
char trg[52];

void set(int i, int val)
{
    if(i == 0)
        return;
    else if(cur[i] == val)
        set(i-1, '0');
    else
    {
        set(i-1, '1');
        cur[i] = val;
        ans += (1ull << (i - 1));
        cur[i-1] = '0';
    }
}

int main()
{
    N = ans = 0;
    scanf("%d %s %s", &N, cur+1, trg+1);
    for(int i = N; i; i--)
    {
        if(cur[i] != trg[i])
        {
            set(i-1, '1');
            cur[i] = trg[i];
            ans++;
        }
    }
    printf("%llu", ans);
}
