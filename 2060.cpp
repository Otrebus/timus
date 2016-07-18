/* 2060. Subpalindrome pairs - http://acm.timus.ru/problem.aspx?num=2060
 *
 * Strategy:
 * First, record the size of the largest palindrome with center at every point inside the string
 * using Manacher's algorithm. Then use this information to calculate how many palindromes start
 * and end at any given position between the characters, as follows: 
 *   Given, say, the string "abcdcba", at every point between the first 'a' and the 'd', two new
 * palindromes will start - the single-letter palindrome ('a' to 'd'), as well as the subpalindrome
 * of the entire string that begins at that point ("abcdcba", "bcdcb", etc). From the 'd' to the
 * the second 'a', one palindrome will start at each character, which is the single-character
 * palindrome consisting of that character. Similarly, at every point just after the first 'a', and
 * just after the 'd', one single-letter palindrome will end, and at every point between the 'd'
 * and the second 'a', two palindromes will end. 
 *   To calculate this quickly for the starting positions (the ending positions are done similarly),
 * for each longest palindrome P at position i in the input string, we mark where P begins such a
 * sequence of characters where at each point a new subpalindrome starts (this is at position i-L 
 * where L is the length of P), and where such a sequence ends. After we've done so for every
 * palindrome, we can then "integrate" this to get the total amount of palindromes that start at any
 * given point.
 *   After this we go through each point between the characters in the string and multiply together 
 * the number of palindromes that start and begin at such a point, and sum all those products
 * together to get the final result.
 *
 * Performance:
 * Linear in the size of the input, runs the tests in 0.015s using 8,092KB memory.
 */

#include <stdio.h>
#include <algorithm>

const int maxn = 300000;

char input[maxn+1], A[3+maxn*2];
int P[3+maxn*2], S[3+maxn*2], E[3+maxn*2];

int main()
{
    int N, n = 0, C = 1, R = 1, x = 0, y = 0;
    scanf("%s%n", input, &N);
    for(int i = 0; i < N; i++)
        A[n+=2] = input[i];
    A[0] = '£', A[++n+1] = '$';

    for(int i = 1; i <= n; i++) // Manacher's algorithm
    {
        P[i] = std::max(std::min(R-i, P[2*C-i]), 0);
        while(A[i+1+P[i]] == A[i-1-P[i]])
            P[i]++;
        if(P[i] + i > R)
            R = i + P[C=i];
    }

    for(int i = 1; i <= n; i++) // Record where the start and end sequences start and end
        S[i-P[i]]++, S[i+!(i%2)]--, E[i+P[i]]++, E[i-!(i%2)]--;
    auto integrate = [] (int& res, int i, int* A) 
                        { int s = A[i]; A[i] += res; res += s; };
    for(int i = 1; i <= n; i++) // Treat the above as "impulses" and "integrate"
        integrate(x, i, S);
    for(int i = n; i >= 1; i--)
        integrate(y, i, E);

    long long sum = 0;
    for(int i = 1; i <= n; i+=2)
        sum += (long long) S[i]*E[i];
    printf("%lld", sum);
}