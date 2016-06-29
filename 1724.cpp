/* 1724. Origin of Man Clarified - http://acm.timus.ru/problem.aspx?num=1724
 *
 * Strategy:
 * First, run through the entire string using the standard algorithm to match braces in a string
 * where we push a brace whenever it's an opening brace (in this case a capital letter) and pop
 * the top two braces whenever they match (in this case an 'Aa' sequence). Record the height of
 * the stack at each point in the input string, and while we do this we also mark for every point
 * in the input sequence the next time the stack drops below this height. For instance, for the
 * sample input:
 *
 * position:    1  2  3  4  5  6  7  8  9 10 11 12 13 14
 * input:       c  a  A  g  t  G  T  t  g  A  a  c  C  c
 * charstack:                     T
 *                    A        G  G  G     A        C  c
 * next:        1  2  4  4  5 12  9  8  9 12 11 12 14 14
 *
 * If we query between any two points, we can use this data to see if those points represent a
 * legal sequence of popping and pushing (if the stack ends up at the same level and never drops
 * below this level between the two points), and hence a legal sequence of letters.
 *
 * Performance:
 * O(L + q), runs the tests in 0.031s using 1,508KB memory (rated 3/377).
 */

#include <algorithm>
#include <stdio.h>
#include <cctype>

const int maxn = 100000;

char input[maxn+2], charstack[maxn+1];
int stack[maxn+1];
int next[maxn+1];
int head[maxn+1]; // These keep track all the values that need their next[] value filled in
int prev[maxn+1]; // Same

int main()
{   
    std::fill(next, next+maxn+1, maxn+2);
    int N, top = 0, q = 0, x, y;
    scanf("%s%n%d", input+1, &N, &q);

    // Goes through the linked list of elements and assign their next[] to the given index
    auto assignnext = [] (int top, int i) 
    { 
        for(int h = head[top]; h; h = prev[h])
            next[h] = i;
        head[top] = 0;
    };

    for(int i = 1; i <= N; i++)
    {
        char c = input[i];
        if(isupper(c))
        {
            // Add this stack height to the bunch of heights that need to know when the next
            // time the stack drop belows their height
            prev[i] = head[top];
            head[top] = i;
            stack[i] = ++top;
            charstack[top] = c;
        }
        else
        {
            charstack[++top] = input[next[i] = i];
            while(islower(charstack[top]))
                // If the top two elements match, peruse the matching prev[]s to assign the next[]s
                if(top && charstack[top-1] == toupper(charstack[top]))
                    assignnext(top-1, i), stack[i] = top -= 2;
                else
                    while(top > 0) // If they don't match, the stack is entirely reset
                        assignnext(--top, i);
        }
    }
    while(q--)
    {
        scanf("%d %d", &x, &y);
        printf(stack[x-1] == stack[y] && next[x] > y ? "1" : "0");
    }
}