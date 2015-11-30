/* 1189. Pairs of Integers - http://acm.timus.ru/problem.aspx?num=1189
 *
 * Strategy:
 * For a given input N, with 5 digits, say, we are to find:
 *    A*10^5 + B*10^4 + C*10^3 + D*10^2 + E*10^1 + F +
 *             A*10^4 + B*10^3 + D*10^2 + E      + F
 *  = N
 * First we see that A needs to be the first digit of N (or one digit less and we need to carry a 
 * 1 on the next step). Then for this value of A, we check the next digit (for value 10^4) and see
 * that A + B must be equal to the next digit of N (unless we carry one), etc. Then, eventually we
 * skip a digit from the second term above (in this case we skip C and end up at D), and now we only
 * have to match 2*D = the fourth digit of N (unless we carry), and then 2*E = the fifth digit of N,
 * etc, all the way to the end. In the code below, the first part, before we skip one digit, is 
 * handled by the function preSkip, and the part after skipping the digit is handled by postSkip.
 * 
 * Performance:
 * O(d*2^d) where d is the number of digits of N, since preSkip branches at most twice for each
 * digit, and each such branch entails a constant number of calls of the linear function postSkip.
 * The solution runs in 0.015s using 420KB memory.
 */

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>

int p[] = { 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000 };
int num[10];
std::vector<int> ans;

void postSkip(int l, int carry, int res)
{
    if(l == -1)
    {
        if(!carry)
            ans.push_back(res);
        return;
    }
    int X = carry*10 + num[l];
    int Y = X/2;
    if(X % 2 == 0)
        postSkip(l-1, 0, res + p[l]*Y);
    Y = (X - 1)/2;
    if((X - 1) % 2 == 0)
        postSkip(l-1, 1, res + p[l]*Y);
}

void preSkip(int l, int carry, int X, int res)
{
    if(l == -1)
        return;
    int Z = carry*10 + num[l];
    int Y = Z - X;
    if(Y >= (!res ? 1 : 0) && Y < 10)
    {
        preSkip(l-1, 0, Y, res + p[l]*Y);
        postSkip(l-1, 0, res + p[l]*Y);
    }
    Y = Z - X - 1;
    if(Y >= (!res ? 1 : 0) && Y < 10)
    {
        preSkip(l-1, 1, Y, res + p[l]*Y);
        postSkip(l-1, 1, res + p[l]*Y);
    }
}

int main()
{
    int n;
    std::cin >> n;
    
    int i = 0;
    for(int k = n; k; k /= 10, i++)
        num[i] = k % 10;
    preSkip(i-1, 0, 0, 0);
    if(num[i-1] == 1)
        preSkip(i-2, 1, 0, 0);
    std::sort(ans.begin(), ans.end());
    int size = std::unique(ans.begin(), ans.end()) - ans.begin();
    std::cout << size << "\n";
    for(int j = 0; j < size; j++)
        std::cout << ans[j] << " + " << std::setfill('0') 
                  << std::setw(std::to_string(ans[j]).length()-1) 
                  << (n-ans[j]) << " = " << n << "\n";
}