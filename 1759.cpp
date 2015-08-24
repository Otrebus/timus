/* 1759. Long-Livers - http://acm.timus.ru/problem.aspx?num=1759
 *
 * Strategy:
 * Calculate the day distance by calculating the number of days from year 0 to date 1 and date 3
 * and taking the difference of these two.
 *
 * Performance:
 * Constant, runs in 0.001s, uses 200KB memory.
 */

#include <stdio.h>
#include <string>

const int monthDays[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

int nDays(int day, int month, int year)
{
    int ret = 365*(year-1) + (year-1)/4 - (year-1)/100 + (year-1)/400;
    for(int i = 1; i < month; i++)
        ret += monthDays[i];
    return ret + day + int(month > 2 && year % 4 == 0 && !(year % 100 == 0 && year % 400 != 0));
}

int main()
{
    int d1, m1, y1, d2, m2, y2, N;
    scanf("%d", &N);
    int max = 0, maxi = 1;
    for(int i = 1; i <= N; i++)
    {
        scanf("%d.%d.%d %*s %d.%d.%d", &d1, &m1, &y1, &d2, &m2, &y2);
        int days = 1 + nDays(d2, m2, y2) - nDays(d1, m1, y1);
        if(days > max)
        {
            max = days;
            maxi = i;
        }
    }
    printf("%d\n", maxi);
    return 0;
}