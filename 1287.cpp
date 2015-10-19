/* 1287. Mars Canals - http://acm.timus.ru/problem.aspx?num=1287
 *
 * Strategy:
 * Keep track of the current diagonal and vertical sequences as seen north of each row as we read
 * it; update this information with the information given by the next row.
 *
 * Performance:
 * O(N^2) using O(N) extra memory, runs the test cases in 0.218s and uses 296KB memory.
 */

#include <stdio.h>
#include <queue>
#include <cctype>
#include <cstring>

struct entry // Represents the three nonhorizontal running sequences as seen north from one square
{
    entry() { std::memset(e, 0, sizeof(e)); }

    // { type, length of sequence }, index 0 is northwest/southeast, 1 is north/south, etc
    std::pair<char, int> e[3];
};

int main()
{
    int N, maxn = 0, maxm = 0;
    char maxc = '?';

    scanf("%d", &N);
    std::vector<entry> P(N);
    std::queue<entry> q; // To minimize memory use, update the new information directly into the
                         // same row, but delayed by two elements; this delay is realized by a queue
    for(int i = 0; i < N; i++)
    {
        int w = 0, m = 0;
        char prev = 0;
        for(int j = 0; j < N; j++)
        {
            // Read the next char of the row
            char c;
            while(isspace(c = getc(stdin)));
            entry e;

            // Check horizontal sequence
            if(j > 0 && c == prev)
            {
                m++;
                if(m > maxn)
                {
                    maxn = m;
                    maxc = prev;
                }
                else if(m == maxn && maxc != prev)
                    maxc = '?';
            }
            else
                m = 1;

            // Check vertical and diagonal sequences of the previous row
            for(int k = -1; k <= 1; k++) // k = -1 is southeast direction, k = 0 is vertical, etc
            {
                // If the previous element northwest/north/southeast of this element had a sequence
                // running the same direction, update this row with that information + 1
                if(j + k >= 0 && j + k < N && c == P[j+k].e[1+k].first)
                    e.e[1+k] = { c, P[j+k].e[1+k].second + 1 };
                else
                    e.e[1+k] = { c, 1 };
                if(e.e[1+k].second > maxn)
                {
                    maxn = e.e[1+k].second;
                    maxc = e.e[1+k].first;
                }
                else if(e.e[1+k].second == maxn && e.e[1+k].first != maxc)
                    maxc = '?';
            }
            q.push(e);
            if(q.size() > 2) // We can start overwriting this row now
            {
                P[w++] = q.front();
                q.pop();
            }
            prev = c;
        }
        while(!q.empty()) // At the end of the line there will still be some information left in
        {                 // the delay queue
            P[w++] = q.front();
            q.pop();
        }
    }
    printf("%c\n%d\n", maxc, maxn);
}