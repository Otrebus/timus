/* 1262. Pseudo-Roman Number - http://acm.timus.ru/problem.aspx?num=1262
 *
 * Strategy:
 * Each digit can not clash with any adjoining digits given the (redundant and overcomplicated) 
 * rules, so we can simply scan through the string and replace it with a given pseudo numeral 
 * that fulfills the rules in isolation. For example, the digit 7*10^x can be replaced by 
 * [5*10^x][1*10^x][1*10^x] so the amount of letters in this numeral is 3.
 *
 * Performance:
 * O(n), runs in 0.001s and uses 192KB of memory.
 */

#include <stdio.h>

char v[10] = { 0, 1, 2, 3, 2, 1, 2, 3, 4, 2}; // Number of letters in each numeral

int main()
{
    int ans = 0;
    char input[2004];
    scanf("%s", input);
    for(int i = 0; input[i]; i++)
        ans += v[(input[i]-'0')];
    printf("%d", ans);
    return 0;
}