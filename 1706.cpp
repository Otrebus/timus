/* 1706. Cipher Message 2 - http://acm.timus.ru/problem.aspx?num=1706
 *
 * Strategy:
 * For each substring of size k, construct the suffix array for that substring and use that and lcp
 * to count the number of unique substrings.
 *
 * Performance:
 * O(nklog k), runs the test cases in 0.998s using 448KB memory.
 */

#include <string>
#include <array>
#include <iostream>
#include <algorithm>

const int maxn = 4000, maxk = 1000;

std::string input;
std::array<int, maxn+maxk+1> h;
std::array<int, maxn+maxk+1> S;
std::array<int, maxn+maxk+1> T;
std::array<std::array<int, maxn+maxk+1>, 13> L;

void sumHist(int length)
{   
    int sum = 0;
    for(int i = 0; i < length; i++)
    {
        int hi = h[i];
        h[i] = sum;
        sum += hi;
    }
}

void init(std::string input)
{   
    std::fill(h.begin(), h.end(), 0);
    std::fill(L[0].begin(), L[0].end(), 0);
    int length = input.length();
    for(int i = 0; i < length; i++)
        h[input[i]-'a']++;
    sumHist(1+'z'-'a');
    for(int i = 0; i < length; i++)
        S[h[input[i]-'a']++] = i;
    L[0][S[0]] = 1;
    for(int i = 1; i < length; i++)
        L[0][S[i]] = input[S[i]] == input[S[i-1]] ? L[0][S[i-1]] : L[0][S[i-1]]+1;
}

int constructSuffixArray(std::string input) // See 1517.cpp for comments about this algorithm
{
    init(input);
    int length = input.length(), w;
    for(w = 1; 1 << (w-1) < length; w++)
    {
        std::fill(h.begin(), h.end(), 0);
        for(int i = 0; i < length; i++)
            h[S[i] + (1 << (w-1)) < length ? L[w-1][S[i] + (1 << (w-1))] : 0]++;
        sumHist(length+1);
        for(int i = 0; i < length; i++)
            T[S[i] + (1 << (w-1)) < length ? h[L[w-1][S[i] + (1 << (w-1))]]++ : h[0]++] = S[i];
        std::fill(h.begin(), h.end(), 0);
        for(int i = 0; i < length; i++) 
            h[L[w-1][T[i]]]++;
        sumHist(length+1);
        for(int i = 0; i < length; i++)
            S[h[L[w-1][T[i]]]++] = T[i];
        L[w][S[0]] = 1;
        for(int i = 1; i < length; i++)
            L[w][S[i]] = L[w-1][S[i]] == L[w-1][S[i-1]] 
                            && (S[i] + (1 << (w-1)) < length && S[i-1] + (1 << (w-1)) < length 
                            && L[w-1][S[i] + (1 << (w-1))] == L[w-1][S[i-1] + (1 << (w-1))])
                                 ? L[w][S[i-1]] : L[w][S[i-1]]+1;
    }
    return w-1;
}

int lcp(int x, int y, int N, int w)
{
    int k, ret = 0;
    for(k = w; k >= 0 && x < N && y < N; k--)
    {
        if(L[k][x] == L[k][y])
        {
            x += 1 << k; 
            y += 1 << k; 
            ret += 1 << k; 
        } 
    }
    return ret;
}

int nsub(int w, int k)
{
    int sum = input.length() - S[0];
    for(int i = 1; i < input.length(); i++)
        sum += input.length()-S[i]-lcp(S[i-1], S[i], input.length(), w);
    return sum;
}

int main()
{
    std::ios::sync_with_stdio(false);
    int k;
    std::string in;
    std::cin >> k >> in;
    for(int i = 0; i < k; i++)
        in.push_back(in[i]);
    for(int i = 0; i < in.length()-k; i++)
    {
        input = in.substr(i, k);
        int w = constructSuffixArray(input);
        std::cout << nsub(w, k) << " ";
    }
}