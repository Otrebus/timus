/* 1517. Freedom of Choice - http://acm.timus.ru/problem.aspx?num=1517
 *
 * Strategy:
 * Construct the suffix array for line1+'@'+line2, and use it to quickly calculate the longest
 * common prefix of each lexicographically adjacently ranked pair of suffixes that come from 
 * opposide sides of the '@'. The longest of such common prefixes is the answer.
 * The suffix array is constructed by sorting the suffixes only by their 2^n-length prefixes
 * for n = 0 and up, and each iteration over a new power 2^n constructs this in linear time 
 * by using the earlier iteration of powers 2^(n-1), leading to an O(n log n) algorithm. 
 * See for example http://www.geeksforgeeks.org/suffix-array-set-2-a-nlognlogn-algorithm/ for an
 * explanation of a version of this algorithm that runs each iteration in linearithmic time.
 *
 * Performance:
 * O(n log n), running the tests in 0.327s using 16828KB memory.
 */

#include <stdio.h>
#include <cstring>
#include <algorithm>

char input[200002];
int h[200002] = { 0 }; // Histogram
int S[200002];         // Explicit lexicographic ordering of each suffix
int T[200002];         // Temporary used for radix sorting of the above
int L[18][200002];     // Mapping from suffix to position for each length 2^0 to 2^18

void sumHist(int length) // Makes a histogram into a cumulative histogram
{   
    int sum = 0;
    for(int i = 0; i < length; i++)
    {
        int hi = h[i];
        h[i] = sum;
        sum += hi;
    }
}

void init(char* input, int length) // Initializes the first iteration of the suffix array algorithm
{   
    for(int i = 0; i < length; i++)
        h[input[i]-'@']++;
    sumHist(1+'Z'-'@');
    for(int i = 0; i < length; i++)
        S[h[input[i]-'@']++] = i;
    L[0][S[0]] = 1;
    for(int i = 1; i < length; i++)
        L[0][S[i]] = input[S[i]] == input[S[i-1]] ? L[0][S[i-1]] : L[0][S[i-1]]+1;
}

void constructSuffixArray(char* input, int length)
{
    init(input, length);
    for(int w = 1; 1 << (w-1) < length; w++)
    {   // Each iteration is in essence a radix sort of two digits
        std::memset(h, 0, (length+1)*sizeof(int));
        // First, sort by the rightmost "digit" - the rank of of the rightmost half of each suffix
        // of power 2^n, which was calculated last iteration as power 2^(n-1)
        for(int i = 0; i < length; i++) // Create histogram of ranks
            h[S[i] + (1 << (w-1)) < length ? L[w-1][S[i] + (1 << (w-1))] : 0]++;
        sumHist(length+1);
        for(int i = 0; i < length; i++) // Populate the array from the histogram
            T[S[i] + (1 << (w-1)) < length ? h[L[w-1][S[i] + (1 << (w-1))]]++ : h[0]++] = S[i];
        std::memset(h, 0, (length+1)*sizeof(int));
        // Now, sort by the leftmost "digit", similarly to earlier
        for(int i = 0; i < length; i++) 
            h[L[w-1][T[i]]]++;
        sumHist(length+1);
        for(int i = 0; i < length; i++) // Populate the array from the histogram
            S[h[L[w-1][T[i]]]++] = T[i];
        L[w][S[0]] = 1;
        // Calculate new ranks, make sure that two equal and adjacent suffixes get the same rank
        for(int i = 1; i < length; i++)
            L[w][S[i]] = L[w-1][S[i]] == L[w-1][S[i-1]] 
                            && (S[i] + (1 << (w-1)) < length && S[i-1] + (1 << (w-1)) < length 
                            && L[w-1][S[i] + (1 << (w-1))] == L[w-1][S[i-1] + (1 << (w-1))])
                                 ? L[w][S[i-1]] : L[w][S[i-1]]+1;
    }
}

int lcp(int x, int y, int N, int w) // Calculates the longest common prefix
{                                   // of two suffixes of length 2^w
    int k, ret = 0;
    for(k = w; k >= 0 && x < N && y < N; k--)
    {
        // If these two suffixes have the same rank, they are equal up to this length, so
        // chop off the length (by increasing the suffix offset) and keep measuring
        if(L[k][x] == L[k][y])
        {
            x += 1 << k; 
            y += 1 << k; 
            ret += 1 << k; 
        } 
    }
    return ret;
}

int main()
{
    int len;
    scanf("%d", &len);
    scanf("%s %s", input, input + len + 1);
    input[len] = '@';

    constructSuffixArray(input, len*2+1);

    int w = 1; // Gives the smallest 2^w that the input fits within
    while(1 << w < len*2+1)
        w++;

    int maxi = 0;
    int maxl = 0;
    // Calculate the longest common prefix of each adjacent suffix in the lexicographic
    // ordering of all suffixes. The longest of all such common prefixes is our answer
    for(int i = 0; i < len*2+1; i++)
    {
        if(S[i] < len && S[i+1] > len || S[i] > len && S[i+1] < len)
        {
            int lp = lcp(S[i], S[i+1], len*2+1, w);
            if(lp > maxl)
            {
                maxl = lp;
                maxi = S[i];
            }
        }
    }
    input[maxi+maxl] = 0;
    printf("%s\n", input+maxi);
    return 0;
}