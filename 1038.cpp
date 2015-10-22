/* 1038. Spell Checker - http://acm.timus.ru/problem.aspx?num=1038
 *
 * Strategy:
 * N/A.
 *
 * Performance:
 * Linear in the size of the input, runs in 0.015s using 180KB memory.
 */

#include <stdio.h>

int main()
{
    bool firstSentence = true, firstWord = true;
    char c;
    int errors = 0;
    while((c = getc(stdin)) != EOF)
    {
        if(c == '.' || c == '!' || c == '?')
            firstSentence = firstWord = true;
        else if((c < 'a' || c > 'z') && (c < 'A' || c > 'Z'))
            firstWord = true;
        else
        {
            if(!firstWord && c >= 'A' && c <= 'Z' || firstSentence && c >= 'a' && c <= 'z')
                errors++;
            firstWord = firstSentence = false;
        }
    }
    printf("%d\n", errors);
    return 0;
}