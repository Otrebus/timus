/* 2081. Faulty dial - http://acm.timus.ru/problem.aspx?num=2081
 *
 * Strategy:
 * Greedy; start with the time 59:59 and for each timer reading given, check if the time could be
 * represented by that timer reading. If not, subtract one second from the time and try again. If a
 * time could be represented, output that time and fetch the next timer reading.
 *
 * Performance:
 * O(n), runs the tests in 0.015s using 172KB memory.
 */

#include <stdio.h>

char digits[3][31] = { "._....._.._....._.._.._.._.._.",
                       "|.|..|._|._||_||_.|_...||_||_|",
                       "|_|..||_.._|..|._||_|..||_|._|", };

bool digitFits(int d, char input[3][18], int pos)
{
    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 3; j++)
            if(digits[i][j+d*3] == '.' && (input[i][j+pos] == '_' || input[i][j+pos] == '|'))
                return false;
    return true;
}

bool fits(int min, int sec, char input[3][18])
{
    return digitFits(min/10, input, 0) && digitFits(min%10, input, 4) &&
           digitFits(sec/10, input, 10) && digitFits(sec%10, input, 14);
}

int main()
{
    int n, min = 59, sec = 60;
    char input[3][18];
    scanf("%d", &n);
    while(n--)
    {
        scanf("%s %s %s", input[0], input[1], input[2]);
        do
            if(--sec < 0)
                min--, sec = 59;
        while(!fits(min, sec, input));
        printf("%02d:%02d\n", min, sec);
    }
}