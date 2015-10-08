/* 1279. Warehouse - http://acm.timus.ru/problem.aspx?num=1279
 *
 * Strategy:
 * Sort the input, and then do a kind of "flood fill" while iterating over the input.
 *
 * Performance:
 * Linearithmic in the size of the input, running the tests in 0.015s using 296KB memory.
 */

#include <stdio.h>
#include <cctype>
#include <vector>
#include <algorithm>

inline int getint()
{
    int ret = 0, ch;
    while(!isdigit(ch = getc(stdin)));
    ret = ch - '0';
    while(isdigit(ch = getc(stdin)))
        ret = ret*10 + ch - '0';
    return ret;
}

int main()
{
    int N = getint(), M = getint(), K = getint();
    std::vector<int> input;
    input.reserve(N*M);
    for(int i = 0; i < N*M; i++)
        input.push_back(getint());
    std::sort(input.begin(), input.end());
    struct entry { int w, h; }; // Number of sections of this height, difference to next height
    std::vector<entry> v;
    for(int i = 0; i < input.size()-1; i++)
        if(input[i] != input[i+1])
            v.push_back( { i+1, input[i+1]-input[i] } );
    v.push_back( { input.size(), std::numeric_limits<int>::max()/input.size() } );
    int l = 0; // l is the added minimum height
    for(int i = 0; i < v.size(); i++)
    {
        if(K >= v[i].w*v[i].h)
            l += v[i].h;
        else
            l += K/v[i].w;
        K -= std::min(K, v[i].w*v[i].h);
    }
    printf("%d\n", l + input.front());
    return 0;
}