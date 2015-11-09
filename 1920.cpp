/* 1920. Titan Ruins: the Infinite Power of Magic - http://acm.timus.ru/problem.aspx?num=1920
 *
 * Strategy:
 * Trace a path that consists of three segments, at any point of these segments returning back
 * to the start point (top left) once the path will be long enough:
 *  1) Straight down
 *  2) Winding back row by row upwards
 *  3) If applicable, the upper two rows alternating from the right to the left
 *
 * Performance:
 * O(L), running the test suite in 0.015s using 204KB memory.
 */

#include <stdio.h>

int N, L, i = 1, j = 1;

void goBack() // Returns back to the start point
{
    while(i > 1)
    {
        printf("%d %d\n", i, j);
        if(i >= 0)
            i--;
    }
    while(j > 1)
    {
        printf("%d %d\n", i, j);
        if(j > 1)
            j--;
    }
}

void goLeft() // Segment 3 in the header comment
{
    int dir = -1;
    while(L != 0)
    {
        if(dir == -1 && i + j - 2 == L)
        {
            goBack();
            return;
        }
        L--;
        if(dir == 1)
        {
            if(i == 1)
                printf("%d %d\n", i++, j);
            else if(i == 2)
            {
                printf("%d %d\n", i, j--);
                dir = -1;
            }                
        }
        else if(dir == -1)
        {
            if(i == 2)
                printf("%d %d\n", i--, j);
            else if(i == 1)
            {
                printf("%d %d\n", i, j--);
                dir = 1;
            }  
        }
    }
}

void goUp() // Segment 2
{
    int dir = 1;
    while(L != 0)
    {
        if(i + j - 2 == L)
        {
            goBack();
            return;
        }
        L--;
        if(dir == 1)
        {
            if(j < N)
                printf("%d %d\n", i, j++);
            else if(j == N)
            {
                printf("%d %d\n", i--, j);
                dir = -1;
                if(i == 2 && N % 2)
                    goLeft();
            }                
        }
        else if(dir == -1)
        {
            if(j > 2)
                printf("%d %d\n", i, j--);
            else if(j == 2)
            {
                printf("%d %d\n", i--, j);
                dir = 1;
            }  
        }
    }
}

void goDown() // Segment 1
{
    while(i <= N)
    {
        L--;
        printf("%d %d\n", i, j);
        if(L == i)
        {
            j++;
            goBack();
            return;
        }
        if(i < N)
            i++;
        else
            break;
    }
    j++;
    goUp();
}

int main()
{
    scanf("%d %d", &N, &L);
    if(L % 2 || L > N*N)
    {
        printf("Unsuitable device\n");
        return 0;
    }
    printf("Overwhelming power of magic\n");
    goDown();
}