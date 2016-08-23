/* 2037. Richness of binary words - http://acm.timus.ru/problem.aspx?num=2037
 *
 * Strategy:
 * To make the strings, a repeated sequence of 'a's is used, interspersed with the string "ababba"
 * which contains six palindromes and introduces no new palindromes centered on it when placed in
 * the middle of a string. This makes it useful to control the number of total subpalindromes in a
 * string.
 *
 * Performance:
 * O(n^2), runs the tests in 0.078s using 260KB memory.
 */

#include <iostream>
#include <string>

char str[7] = "ababba";

// Returns a string with p repetitions of 'a' followed by one sequence of str, repeating for n
// total characters
std::string repeat(int p, int n)
{
    std::string s;
    bool loop = true;
    for(int i = 0; i < n; )
    {
        for(int j = 0; j < p && i < n; i++, j++)
            s.push_back('a');
        for(int j = 0; j < 6 && i < n; i++, j++)
            s.push_back(str[j]);
    }
    return s;
}

std::string print(int n, int k)
{
    if(n == 8 && k == 7)
        return "aababbaa"; // The only true special case
    if(n != k && k < 8)
        return "NO";
    if(k <= (n - 6)/2 + 6) // We can place "abaaba" once in the string to satisfy the request
        return "a" + repeat(k-8, n-1);
    return repeat(k-6, n); // Here we have to repeat it - between the "abaaba"'s we get one extra
}                          // palindrome and have to adjust for that

int main()
{
    std::ios::sync_with_stdio(false);
    int n;
    std::cin >> n;
    for(int k = 1; k <= n; k++)
        std::cout << k << " : " << print(n, k) << "\n";
}