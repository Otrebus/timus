/* 1334. Checkers - http://acm.timus.ru/problem.aspx?num=1334
 *
 * Strategy:
 * Straight implementation of the rules.
 *
 * Performance:
 * Constant, runs the tests in 0.015s using 200KB memory.
 */

#include <stdio.h>
#include <array>

char board[8][8];

bool inbounds(int x, int y)
{
    return x >= 0 && x < 8 && y >= 0 && y < 8;
}

bool check(int x, int y) // Checks if the checker at this position can be flipped
{
    int c = board[x][y];
    for(int i : { -1, 1 } )
        for(int j : {-1, 1 } )
             if(inbounds(x+i, y+j) && inbounds(x+i*-1, y+j*-1) && 
                board[x+i][y+j] == 1 + (c % 2) && board[x+i*-1][y+j*-1] == 0)
                return true;
    return false;
}

int run()
{
    char c = 1;
    for(int i = 1; i <= 32; i++)
    {
        char s[3];
        scanf("%s", s);
        board[s[0]-'a'][s[1]-'1'] = c = 1 + (c % 2);
        for(int x = 0; x < 8; x++) // Overkill, but the input is small
            for(int y = 0; y < 8; y++)
                if(board[x][y] && check(x, y))
                    return i;
    }
    return 0;
}

int main()
{
    int result = run();
    printf(!result ? "Draw" : "%d", result);
    return 0;
}
