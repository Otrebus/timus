/* 1732. Ministry of Truth - http://acm.timus.ru/problem.aspx?num=1732
 *
 * Strategy:
 * Greedily match each word of the second line to the earliest possible point in the first line,
 * perform each string matching with the KMP algorithm.
 * 
 * Performance:
 * Linear in the size of the input, runs the test cases in 0.015s using 752KB memory.
 */

#include <stdio.h>

int kmp[100001]; // KMP failure array
char T[100001]; // First uttering, will be partially overwritten with _'s
char P[100001]; // Second uttering

int preKmp(char* p) // KMP array preprocessing
{
    int k = 0, q;
    kmp[1] = 0;
    for(q = 1; p[q] && p[q] != ' '; q++)
    {
        while(k > 0 && p[k] != p[q])
            k = kmp[k];
        if(p[k] == p[q])
            k++;
        kmp[q+1] = k;
    }
    return q;
}

int match(char* t, char* p) // KMP string matching
{
    if(!*p)
        return -1;
    preKmp(p);
    int q = 0;
    for(int i = 0; t[i] && t[i] != ' '; i++)
    {
        while(q > 0 && p[q] != t[i])
            q = kmp[q];
        if(p[q] == t[i])
            q++;
        if(!p[q] || p[q] == ' ')
            return i-(q-1);
    }
    return -1;
}

void skipws(char*& t)
{
    while(*t == ' ')
        t++;
}

int main()
{
    char* t = T;
    char* p = P;
    gets(T);
    gets(P);
    skipws(t); skipws(p);

    while(*t)
    {
        int m = match(t, p), l;
        if(m > -1) // We matched one word, blank out everything previous
        {          // to the match, and blank out one letter after
            for(l = 0; p[l] && p[l] != ' '; l++); // Length of the matched word
            for(int i = 0; i < m; i++)
                t[i] = '_';
            p += l;
            t += l+m;
            if(*p && *t)
                *t = *t != ' ' ? '_' : ' ';
        }
        else // No match, blank out everything and try again on the next word
        {
            for(l = 0; t[l] && t[l] != ' '; l++);
            for(int i = 0; i < l; i++)
                t[i] = '_';
            t += l;
        }
        skipws(t); skipws(p);
    }
    if(!*p)
        printf("%s\n", T);
    else
        printf("I HAVE FAILED!!!\n");
        
    return 0;
}