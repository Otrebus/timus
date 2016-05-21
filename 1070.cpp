/* 1070. Local Time - http://acm.timus.ru/problem.aspx?num=1070
 *
 * Strategy:
 * Iterate through the possible time differences, and see which one makes sense from the given
 * constraints.
 *
 * Performance:
 * Constant, runs the test cases in 0.001s using 172KB memory.
 */

#include <cmath>
#include <stdio.h>

int dif(int h1, int m1, int h2, int m2)
{
    int d = h2*60+m2 - (h1*60+m1);
    if(d > 60*12)
        d -= 60*24;
    else if(d < -60*12)
        d += 60*24;
    return d;
}

int main()
{
    int d1, d2;
    int h1, m1, h2, m2;
    scanf("%d.%d %d.%d", &h1, &m1, &h2, &m2);
    d1 = dif(h1, m1, h2, m2);
    scanf("%d.%d %d.%d", &h1, &m1, &h2, &m2);
    d2 = dif(h1, m1, h2, m2);

    for(int d = -5; d <= 5; d++)
    {
        if(d1 - d*60 >= 0 && d1 - d*60 <= 6*60 && d2 + d*60 >= 0 
           && d2 + d*60 <= 6*60 && std::abs((d1-d*60) - (d2+d*60)) <= 10)
        {
            printf("%d\n", std::abs(d));
            return 0;
        }
    }
}