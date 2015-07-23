/* 1406. Next Number - http://acm.timus.ru/problem.aspx?num=1406
 *
 * Strategy:
 * Iterate over the number string from right to left, through all initial (rightmost) zeroes, 
 * through some nonzero number and through all following numbers equal to nine. Sum together the 
 * digits of this string, subtract this sum by one and replace the string with the lowest number 
 * constructible from this sum. Increment the next number (which is not a nine) in the string.
 *
 * Performance:
 * O(n), where n is the length of the input string. The solution runs in 0.015s and uses 192KB of
 * memory.
 */

#include <stdio.h>
#include <string>
#include <algorithm>
#include <cstring>

bool next(char* input)
{
    int length = strlen(input);
    for(int i = 0; i < length; i++)
        input[i] -= '0'; // Un-asciify
    int i = length-1;
    while(i > 0 && input[i] == 0)
        i--; // Zoom through all rightmost zeroes
    int sum = input[i--]; // Add the first nonzero number
    if(sum == 0) // Means number was all zeroes
        return false;
    while(input[i] == 9 && i >= 0)
    {
        sum += input[i]; // Add all nines following this
        i--;
    }
    if(i < 0) // Means it was nines all the way down and we would expand when incrementing
        return false;
    sum--;
    int j;
    for(j = length-1; j > i; j--) // Construct the smallest number from this sum
    {
        input[j] = std::min(sum, 9);
        sum -= input[j];
    }
    input[i]++; // Increments the leftmost string
    for(int i = 0; i < length; i++)
        input[i] += '0'; // Asciify
    return true;
}

int main()
{
    char input[2001];
    scanf("%s", input);
    if(!next(input))
        printf("-1\n");
    else
        printf("%s\n", input);
    return 0;
}