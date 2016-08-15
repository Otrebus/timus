/* 1037. Memory Management - http://acm.timus.ru/problem.aspx?num=1037
 *
 * Strategy:
 * Maintain a set containing all free blocks and a queue containing all scheduled timeouts. When
 * a block is allocated or accessed, the timeout and the block are pushed onto a queue; in order to
 * cull timeouts that have been extended from the queue, an array is used to keep track of the
 * actual most recent valid timeout of all blocks.
 *
 * Performance:
 * O(Q log N), where Q is the amount of requests. Runs the tests in 0.249s using 1,988KB memory.
 */

#include <iostream>
#include <queue>
#include <set>

const int maxn = 30000;

int timeout[maxn+1];

int main()
{
    std::ios::sync_with_stdio(false);
    std::set<int> free;

    // Contains the tentative timeouts - blocks that have been extended may appear several times
    // but only the most recent one counts, which is the one in the timeout array
    std::queue<std::pair<int, int>> q;

    for(int i = 1; i <= maxn; i++)
        free.insert(i);

    int t, b;
    char c[2];
    while(std::cin >> t >> c)
    {
        while(!q.empty() && q.front().first <= t) // Go through the timeouts
        {
            auto t = q.front().first, b = q.front().second;
            q.pop();
            if(timeout[b] == t) // Check that this timeout wasn't overridden at some point
                free.insert(b);
        }
        if(c[0] == '+')
        {
            auto it = free.begin();
            auto b = *it;
            free.erase(it);
            timeout[b] = t + 600;
            q.push( { t + 600, b } );
            std::cout << b << "\n";
        }
        else
        {
            std::cin >> b;
            if(free.find(b) != free.end())
                std::cout << "-\n"; // If this block is among the free, it's not allocated
            else
            {
                q.push( { t + 600, b } );
                timeout[b] = t + 600;
                std::cout << "+\n";
            }
        }
    }
}