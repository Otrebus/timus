/* 1959. GOV-internship 3 - http://acm.timus.ru/problem.aspx?num=1959
 *
 * Strategy:
 * Let's consider the case where the pattern string contains zeroes (the other case is similar).
 * If the input is
 *
 *                                          Text:
 *                                          4 2 3 2 3 1
 *                                          Pattern:
 *                                          1 0 0 1
 *
 * Then we see that we will minimize the total Hamming distance that the first zero contributes 
 * towards if we set it to be the most common number (the mode) that it will be compared to among
 * the group { 2, 3, 2 } as we compare the three different valid comparison positions of the
 * pattern. The second zero should be set to the most common number among { 3, 2, 3 }. 
 *   
 * To quickly retrieve which number is the most common from each such sequence, we maintain a 
 * data structure based on continuously building the next sequence from the previous by adding and
 * removing digits from the first sequence (in the above case, we removed a 2 and added a 3) as we
 * scan the text string from the left to the right. The data structure consists of an array of
 * linked lists that maps each currently existing number frequency to the different numbers that
 * make up that frequency. If we create such a data structure for the entire text input above,
 * it would look like:
 *
 *                                             ...
 *                                          3 -> null
 *                                          2 -> 2, 3
 *                                          1 -> 1, 4
 *                                          0 -> null
 *
 * If we add a 1 to this data structure (as we move the segment that each 0 of the pattern draws
 * its mode from), we simply move the 1 from the "1" entry to the "2" entry, and update the pointer
 * to the maximum position, if applicable.
 *
 * Performance:
 * O(n), runs the tests in 0.093s using 2,932KB memory (rated 4/180).
 */

#include <vector>
#include <stdio.h>
#include <algorithm>

const int maxn = 100000;

int max;
int head[maxn+1]; // The head of the linked list of the given frequency
int next[maxn+1]; // The next number in the list
int* prev[maxn+1]; // Whatever is pointing to this number (a head or a 'next' field)
int pos[maxn+1]; // The frequency of a given number

int text[maxn], pattern[maxn];

inline int get()
{
    return head[max];
}

inline void cut(int i) // Cut a number from the linked list
{
    *prev[i] = (next[i]);
    prev[next[i]] = prev[i];
}

inline void paste(int pos, int i) // Paste a number into the frequency list
{
    prev[head[pos]] = &next[i];
    next[i] = head[pos];
    head[pos] = i;
    prev[i] = &head[pos];
}

inline void add(int i) // Add this number to the data structure
{
    cut(i);
    ++pos[i];
    paste(pos[i], i);
    if(pos[i] > max)
        max = pos[i];
}

inline void remove(int i) // Remove this number from the data structure
{
    cut(i);
    if(max == pos[i] && head[pos[i]] == 0)
        max = pos[i]-1;
    --pos[i];
    paste(pos[i], i);
}

int main()
{
    std::vector<int> zeros;
    int n, m;
    bool n0 = false, m0 = false;
    for(int i = 1; i < maxn; i++)
        next[i] = i+1, prev[i+1] = &next[i];
    prev[1] = &head[0];

    scanf("%d", &n);
    for(int i = 0; i < n; i++)
        scanf("%d", &text[i]), n0 |= text[i] == 0;
    scanf("%d", &m);
    for(int i = 0; i < m; i++)
        scanf("%d", &pattern[i]), m0 |= pattern[i] == 0;
    if(n0) // The text array contains the numbers
    {
        for(int i = 0; i < n; i++)
            if(!text[i])
                zeros.push_back(i);
        int j = 0;
        // Scan the pattern and text arrays from the left to the right, adding and removing numbers
        // to the data structure in the process depending on how we encounter zeroes
        for(int i = std::min(m-1, zeros[j]); zeros[j] <= i+n-m && i >= 0; i--)
            add(pattern[i]);
        for(int i = zeros[j]; i < n && j < zeros.size(); i++)
        {
            if(i == zeros[j])
                text[zeros[j++]] = get();
            if(i+1 < m)
                add(pattern[i+1]);
            if(m-n+i >= 0 && i < n)
                remove(pattern[m-n+i]);
        }

    }
    else if(m0) // Similar to the above but here the pattern array contains the zeroes
    {
        for(int i = 0; i < m; i++)
            if(!pattern[i])
                zeros.push_back(i);
        int j = 0;
        for(int i = zeros[j]; i + m - zeros[j] <= n; i++)
            add(text[i]);
        // Essentially we maintain a fixed size segment of the text as a sequence as we scan arrays
        for(int i = zeros[j]; i < n && j < zeros.size(); i++)
        {
            if(i == zeros[j])
                pattern[zeros[j++]] = get();
            if(i+1-m < n)
                add(text[i+1+n-m]), remove(text[i]);
        }
    }
    for(int i = 0; i < n; i++)
        printf("%d ", text[i]);
    printf("\n");
    for(int i = 0; i < m; i++)
        printf("%d ", pattern[i]);
}