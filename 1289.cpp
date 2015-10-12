/* 1289. One Way Ticket - http://acm.timus.ru/problem.aspx?num=1289
 *
 * Strategy:
 * Let the digit square root of some number n be denoted dsqrt(n). For all numbers n that are i
 * digits long, let S_i(k) denote the subset of those numbers that have a dsqrt of k. For every 
 * number i digits long that has a dsqrt of k, appending a digit c to that number gives a number 
 * i+1 digits long that has a dsqrt of (k+c), calculated mod 9, but skipping 0. For example,
 * dsqrt(88) = 1+6 = 7, and dsqrt(884) = dsqrt(8+8+4) = dsqrt(2+0) = 2. This means that for each
 * number i+1 digits long of some dsqrt d, ten different dsqrts of the number i digits long had
 * some 1 <= c <= 9 added to them that contribute to the amount of dsqrts d of length i+1, except 
 * for the number with dsqrt of 0, which only contributed a single number. Hence |S_(i+1)(k)| =
 * 10*|S_(i)(k)| + 1. The amount of 2*i digit numbers that have the dsqrt of the first half and
 * second half equal is |S_(i)(k)|^2 for each k, which gives 9*|S_(i)(k)|^2 in total, plus one
 * for the dsqrt of 0. For each i from 1 and up this gives the results 10, 1090, 110890, 
 * 11108890, 1111088890, etc, which is printed out by the program below.
 *
 * Performance:
 * O(N), running the tests in 0.015s using 200KB memory.
 */

#include <stdio.h>

int main()
{
    int N;
    scanf("%d", &N);
    for(int k = 1; k <= N/2-2; k++)
        printf("1");
    printf("10");
    for(int k = 1; k <= N/2-2; k++)
        printf("8");
    if(N > 2)
        printf("90");
}