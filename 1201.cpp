/* 1201. Which Day Is It? - http://acm.timus.ru/problem.aspx?num=1201
 *
 * Strategy:
 * This problem is sort of boring and was done only for the sake of exhaustivity. The code is
 * correspondingly uninspired, inconsistent, not meant for human digestion, and deserves no comment.
 *
 * Performance:
 * Constant time, technically. Runs in 0.001s and uses no extra memory.
 */

#include <stdio.h>
#include <algorithm>

int md[13] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
char* dn[8] = { "", "mon", "tue", "wed", "thu", "fri", "sat", "sun" };

bool leap(int y)
{
    return (y % 4 == 0 && y % 100 != 0 || y % 400 == 0);
}

int daysinmonth(int y, int m)
{
    if(leap(y) && m == 2)
        return 29;
    return md[m];
}

int getMonthStart(int m, int y)
{
    int t = 5;
    for(int i = 1600; i <= y; i++)
    {
        int M;
        if(i < y)
            M = 12;
        else
            M = m-1;
        for(int j = 1; j <= M; j++)
            t += daysinmonth(i, j);
    }
    return t%7 + 1;
}

void printDay(int i, int wd, int m, int ms, int c, int y, int nRows)
{
    int d = (wd-ms) + (i-1)*7;
    if(d > 0 && d <= daysinmonth(y, m))
    {
        if(d == c)
            printf("[%2d]", d);
        else
        {
            printf(" %2d", d);
            if(i < nRows)
                printf(" ");
        }
    }
    else
    {
        if(i < nRows)
            printf("    ");
        else
            printf("   ");
    }
}

void printRow(int i, int m, int ms, int c, int y)
{
    printf("%s", dn[i]);
    int nRows = (ms-1+daysinmonth(y, m))/7 + ((ms-1+daysinmonth(y, m))%7 ? 1 : 0);
    for(int j = 1; j <= nRows; j++)
    {
        printf(" ");
        printDay(j, i+1, m, ms, c, y, nRows);
    }
    printf("\n");
}

int main()
{
    int d, m, y;
    scanf("%d %d %d", &d, &m, &y);
    int ms = getMonthStart(m, y);
    for(int i = 1; i <= 7; i++)
        printRow(i, m, ms, d, y);
    return 0;
}