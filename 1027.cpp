/* 1027. D++ Again - http://acm.timus.ru/problem.aspx?num=1027
 *
 * Strategy:
 * Naive.
 *
 * Performance:
 * Linear in the size of the input, runs the test cases in 0.015s using 272KB memory.
 */

#include <stdio.h>
#include <string>

char op[8] = "=+-*/";

bool solve(std::string str)
{
    int paren = 0, N = str.size();
    str.push_back(0);
    bool comment = false;
    for(int i = 0; i < N; i++) // Burn the braces!
        if(str[i] == '*' && str[i+1] == ')')
            if(!comment)
                if(paren)
                    continue;
                else
                    return false;
            else
                comment = false, i++;
        else if(comment)
            continue;
        else if(str[i] == '(' && str[i+1] == '*')
            comment = true, i++;
        else if(str[i] == '(')
            paren++;
        else if(str[i] == ')')
            if(!paren)
                return false;
            else
                paren--;
        else if(paren && !(str[i] >= '0' && str[i] <= '9' 
                           || *std::find(op, op+5, str[i])))
            return false;
    if(paren || comment)
        return false;
    return true;
}

int main()
{
    std::string s;
    for(int c = getc(stdin); c != EOF; c = getc(stdin))
        if(c != '\n' && c != '\r')
            s.push_back(c);
    printf(solve(s) ? "YES\n" : "NO\n");
}