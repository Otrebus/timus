/* 2063. Black and White - http://acm.timus.ru/problem.aspx?num=2063
 *
 * Strategy:
 * For each row of n balls, use the queries to bubble sort blindly, and then choose as statement
 * some pair (i,i+1) randomly. The probability of choosing exactly the junction between black and 
 * white balls is 1/(n+1), which over a set of games where n <- [2,99] gives small enough of an 
 * expected value of the number of such unfortunate choices to virtually guarantee 80 correct
 * guesses.
 *
 * Performance:
 * O(n^3), runs the test suite in 0.093s using 292KB memory.
 */

#include <stdio.h>
#include <random>
#include <algorithm>

char dummy[100000];

int main()
{
    std::random_device rd;
    std::mt19937 mt(rd());

    int N;
    scanf("%d", &N);
    for(int i = 0, j = 2; i < N; i++, j++)
    {
        scanf("%s", dummy);

        for(int k = 1; k <= j; k++)
            for(int l = 1; l < j; l++)
                printf("? %d %d\n", l, l+1);
        std::uniform_int_distribution<int> dist(1, j-1);
        int n = dist(mt);
        printf("! %d %d\n", n, n+1);
    }
    return 0;
}