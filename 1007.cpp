/* 1007. Code Words - http://acm.timus.ru/problem.aspx?num=1007
 *
 * Strategy:
 * The length of the code word determines the nature of its corruption, so each case is handled
 * separately. The algorithms are straightforward and fast by using the fact that removing a digit 
 * from a keyword reduces its checksum by the number of ones to the right of it (together with the 
 * effect of the removed digit), and the opposite if we add a digit.
 *
 * Performance:
 * Linear in the size of the input; runs the test cases in 0.015s using 192KB of memory.
 */

#include <stdio.h>
#include <algorithm>

int checksum(const char* str, int len, int N)
{
    int sum = 0;
    for(int i = 0; i < len; i++)
        sum = (sum + (i + 1)*(str[i]-'0')) % (N + 1);
    return sum;
}

void fixshort(const char* str, char* str2, int N)
{
    int chk = checksum(str, N - 1, N), p = 0, ones = 0;
    bool broken = true;
    std::for_each(str, str + N - 1, [&ones] (char c) { if(c == '1') ones++; } );

    for(int i = 0; i < N; i++)
    {
        if(broken && (chk + ones + i + 1) % (N+1) == 0)
        {
            str2[p++] = '1';
            broken = false;
        }
        else if(broken && (chk + ones) % (N+1) == 0)
        {
            str2[p++] = '0';
            broken = false;
        }
        str2[p++] = str[i];
        if(str[i] == '1')
            ones--;
    }
}

void fixlong(const char* str, char* str2, int N)
{
    int chk = checksum(str, N + 1, N), p = 0, ones = 0;
    bool broken = true;
    std::for_each(str, str + N + 1, [&ones] (char c) { if(c == '1') ones++; } );

    for(int i = 0; i < N + 1; i++)
    {
        if(str[i] == '1')
            ones--;
        if(broken && str[i] == '1' && (chk - ones - i - 1) % (N+1) == 0)
        {
            i++;
            broken = false;
        }
        else if(broken && str[i] == '0' && (chk - ones) % (N+1) == 0)
        {
            i++;
            broken = false;
        }
        str2[p++] = str[i];
    }
    str2[p] = 0;
}

void fixeq(const char* str, char* str2, int N)
{
    int chk = checksum(str, N, N);
    bool broken = chk % (N+1) != 0;
    int p = 0;
    for(int i = 0; i < N; i++)
    {
        if(broken && str[i] == '1' && (i + 1) % (N + 1) == chk)
        {
            str2[p++] = '0';
            broken = false;
        }
        else
            str2[p++] = str[i];
    }
    str2[p] = 0;
}

int main()
{
    int N;
    char str[1002], str2[1002];
    scanf("%d", &N);

    while(scanf("%s", str) != EOF)
    {
        int len = str[N-1] == 0 ? N-1 : str[N] == 0 ? N : N+1;
        if(len < N)
            fixshort(str, str2, N);
        else if(len == N)
            fixeq(str, str2, N);
        else if(len > N)
            fixlong(str, str2, N);
        printf("%s\n", str2);
    }
    return 0;
}