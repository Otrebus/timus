/* 1316. Electronic Auction - http://acm.timus.ru/problem.aspx?num=1316
 *
 * Strategy:
 * Maintain a Fenwick tree where each index corresponds to the number of people who have bid exactly
 * that amount of money. For each sale we can then quickly determine how many people have bid more
 * than that amount, which is the central mechanism in determining the final answer.
 *
 * Performance:
 * O(n log d) where n is the number of operations and d is the maximum bid, runs in 0.093s using
 * 4108KB memory.
 */

#include <stdio.h>
#include <algorithm>

int A[1000001];

int sum(int i)
{
    int sum = 0;
    while(i > 0) 
        sum += A[i], i -= i & -i;
    return sum;
}
 
void add(int i, int k)
{
    while (i < 1000001) 
        A[i] += k, i += i & -i;
}

int main()
{
    char s[10];
    double d;
    int k;
    long long res = 0;
    while(true)
    {
        scanf("%s", s);
        if(s[0] == 'B')
        {
            scanf("%lf", &d);
            add(1000001 - (int)(d*100.0 + 0.1), 1);
        }
        else if(s[0] == 'S')
        {
            scanf("%lf %d", &d, &k);
            scanf("%d", &k);
            res += std::max(0, std::min(k, sum(1000001 - (int)(d*100.0+0.1))));
        }
        else if(s[0] == 'D')
        {
            scanf("%lf", &d);
            add(1000001 - (int)(d*100.0 + 0.1), -1);
        }
        else
            break;
    }
    printf("%.2lf\n", res/100.0);
    return 0;
}
