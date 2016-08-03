/* 1230. Introspective Program - http://acm.timus.ru/problem.aspx?num=1230
 *
 * Strategy:
 * Constructed using the approach described in the section on self-reference in Chapter 6.1 of
 * "Introduction to the Theory of Computation" 3rd ed. by Michael Sipser.
 *
 * Performance:
 * N/A.
 */

#include <iostream>

int main()
{
    std::cout << "S=\"'\";D='\"';B=\";A='S='+$(D,1,1)+$(S,1,1)+$(D,1,1)+';D='+$(S,1,1)+$(D,1,1)+"
                 "$(S,1,1)+';B='+$(D,1,1)+B+$(D,1,1);?A;?B\";A='S='+$(D,1,1)+$(S,1,1)+$(D,1,1)+'"
                 ";D='+$(S,1,1)+$(D,1,1)+$(S,1,1)+';B='+$(D,1,1)+B+$(D,1,1);?A;?B";
}