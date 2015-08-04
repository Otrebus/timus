/* 1483. Table Football - http://acm.timus.ru/problem.aspx?num=1483
 *
 * Strategy:
 * The lowest score for any team on first place is if all teams play all draws, and the highest
 * score for any team on last place is if all teams end up on the same place and win against 
 * half of the other teams, and lose against the other teams, with one draw if playing an uneven 
 * amount of teams.
 *
 * Performance:
 * Constant, runs in 0.001s and uses 196KB of memory.
 */

#include <stdio.h>

int main()
{
    int n;
    scanf("%d", &n);
    printf("%d %d\n", n-1, (n-1)/2*3 + (n-1)%2);
    return 0;
}