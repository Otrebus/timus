/* 1410. Crack - http://acm.timus.ru/problem.aspx?num=1410
 *
 * Strategy:
 * Dynamic programming. If A[k] is the answer for the subproblem that considers the first k words,
 * and length(k) is the length of the kth word, we have A[k] = length(k) + max { A[k-2], A[k-3] },
 * since we have to exclude word k-1 and can include either word k-2 or k-3. The final result is then
 * max { A[n-1], A[n] } where n is number of words of the input.
 *
 * Performance:
 * Linear in the size of the input, runs the test cases in 0.031s using 184KB memory.
 */

#include <stdio.h>
#include <algorithm>

inline bool isLetter(char c)
{
    return c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z';
}

int main()
{
    bool hadLetter = false;
    int length = 0, i = 0, c;
    int buf[3] = { 0, 0, 0 }; // Similar to A in the header comment, but as a circular buffer since
    do                        // we only ever need to keep track of the previous three word lenghts
    {
        c = getc(stdin);
        if(!isLetter(c) && hadLetter)
        {
            hadLetter = false;
            buf[i%3] = length + std::max(buf[i%3], buf[(i+1)%3]);
            length = 0;
            i++;
        }
        else if(isLetter(c))
        {
            hadLetter = true;
            length++;
        }
    } while(c != EOF);
    printf("%d\n", *std::max_element(buf, buf+3));
}