/* 1095. Nikifor 3 - http://acm.timus.ru/problem.aspx?num=1095
 *
 * Strategy:
 * Given an input number like 9814730290, form the number 9987000000, which consists of all digits
 * in the input number, except one each from the set {1,2,3,4}, followed by four zeroes (that leave
 * room for that set), followed by the number of zeroes given in the input number. Then add the
 * number A*10^z which brings the modulo to zero, where A is a number formed by the permutation 
 * of the digits of 1234 (in this case A*10^z = 132400, which gives output 9987132400).
 *
 * Performance:
 * O(Nlog n) where n is the maximum value of the input values, runs the test set in 0.078s using
 * 384KB memory.
 */

#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <cstring>

std::string a[7] = { "3241", "1324", "1234", "2341", "1243", "1342", "2134" };

std::string calc(std::string str)
{
    int h[10]; // Histogram of digits
    std::string ret;
    std::memset(h, 0, sizeof(h));
    for(int i = 0; i < str.size(); i++)
        h[str[i]-'0']++;
    for(int i = 1; i <= 4; i++)
        h[i] -= 1; // Discount one each of 1,2,3,4
    int m = 1; // Keeps track of the mod 7 of the powers of 10
    int Z = h[0]; // Number of zeroes in the number
    int tot = 0; // Modulo 7 of the entire number so far
    for(int i = 0; i < Z; i++)
        m = (10*m)%7;
    int z = m; // The modulo of 10^Z
    m = (m*10000)%7; // Add space for the permuted 1234's

    // Calculate the modulo for the tentative output (9987000000 in the header comment example)
    for(int k = 1; k <= 9; k++)
    {
        for(int i = 0; i < h[k]; i++)
        {
            tot = (tot + m*k)%7;
            m = (m*10)%7;
            ret += ('0' + k);
        }
    }
    // Well, actually it was the reverse of it
    std::reverse(ret.begin(), ret.end());

    // Try different instances of 1234*10^Z and their modulos until we find one which is 0
    for(int i = 0; i < 7; i++)
    {
        if((z*i + tot) % 7 == 0)
        {
            ret += a[i];
            for(int i = 0; i < Z; i++)
                ret += '0';
            return ret;
        }
    }
}

int main()
{
    std::ios::sync_with_stdio(false);
    int N;
    std::cin >> N;
    while(N--)
    {
        std::string str;
        std::cin >> str;
        std::cout << calc(str) << "\n";
    }
}