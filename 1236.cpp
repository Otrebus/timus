/* 1236. Decoding Task - http://acm.timus.ru/problem.aspx?num=1236
 *
 * Strategy:
 * The given space character xored with the first byte of the result yields the first byte of the 
 * key. Then, the first byte of the key xored with the first byte of the first string gives us the
 * decoded second byte of the second string, which we can xor with the coded second byte of the
 * second string to give us the second byte of the key, and so on.
 * 
 * Performance:
 * O(N), runs in 0.015s using 240KB memory.
 */

#include <stdio.h>

char a[20001];
char b[20003];

unsigned char hexToChar(char c)
{
    return c >= '0' && c <= '9' ? c - '0' : 10 + c - 'A';
}

unsigned char getHex(char* a)
{
    return 16*(hexToChar(a[0])) + hexToChar(a[1]);
}

int main()
{
    int N;
    scanf("%s%n", a, &N);
    scanf("%s", b);
    unsigned char p = 32;
    unsigned char k = p^getHex(b);
    printf("%02X", (unsigned char) k);
    for(int i = 0; i < N; i+=2)
    {
        unsigned char d = k^getHex(a+i);
        k = d^getHex(b+i+2);
        printf("%02X", (unsigned char) k);
    }
}