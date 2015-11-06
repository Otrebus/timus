/* 1202. Rectangles Travel - http://acm.timus.ru/problem.aspx?num=1202
 *
 * Strategy:
 * Greedy - travel to the next rectangle using the shortest possible path.
 *
 * Performance:
 * O(n), runs the test suite in 0.078s using 200KB memory.
 */

#include <stdio.h>
#include <algorithm>

int main()
{
    int N;
    scanf("%d", &N);
    int posX = 1, posY = 1, ylow = -1000000, yhigh = 1000000, d = 0, x1 = 0, y1 = 0, x2 = 0, y2 = 0;
    while(N--)
    {
        d += x2 - x1;
        posX += x2 - x1;
        scanf("%d %d %d %d", &x1, &y1, &x2, &y2);
        if(y2 < ylow + 2 || y1 > yhigh - 2)
        {
            printf("-1\n");
            return 0;
        }
        else if(posY < y1 + 1)
            d += y1+1 - posY, posY = y1+1;
        else if(posY > y2 - 1)
            d += posY - (y2-1), posY = y2-1;
        ylow = y1;
        yhigh = y2;
    }
    d += x2 - x1 - 2 + std::abs(posY-(y2-1));
    printf("%d\n", d);
    return 0;
}