/* 1574. Mathematicians and brackets - http://acm.timus.ru/problem.aspx?num=1574
 *
 * Strategy:
 * Think of the input string as the graph of a function that jumps up one step whenever an opening
 * parenthesis is encountered, and falls down one step when a closing parenthesis is encountered. 
 * Then, a valid cyclic shift of suffix length n is possible if we can start at the value at
 * position n in the graph and continuously read values (wrapping around) without ever dipping below
 * the starting value. This is possible only at the places the function reaches its minimum value.
 * 
 * Performance:
 * Linear in the length of the string, running the tests in 0.015s using 360 KB memory.
 */

#include <stdio.h>

int main()
{
    int k = 0, min = 0, mincount = 0;
    while(true)
    {
        char c = getc(stdin);
        if(c != ')' && c != '(')
            break;
        k += c == '(' ? 1 : -1;
        if(k < min)
        {
            mincount = 1;
            min = k;
        }
        else if(k == min)
            mincount++;
    }
    printf("%d\n", k == 0 ? mincount : 0);
    return 0;
}