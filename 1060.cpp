/* 1060. Flip Game - http://acm.timus.ru/problem.aspx?num=1060
 *
 * Strategy:
 * From the input position, do a breadth first search using the 16 possible moves and keep distance
 * of each visited position memorized in a cache. Whenever an earlier configuration is encountered
 * it means we have already visited this position with fewer amounts of moves. Each position is
 * represented as a 16-bit value, with b being 1 and w being 0.
 *
 * Performance:
 * O(1), or at most 2^16 iterations since this is the size of the input board. The test cases are 
 * run in 0.001s (which is the fastest time), and uses 332KB memory.
 */

#include <stdio.h>
#include <algorithm>
#include <cstring>

unsigned short mask[16] = {0}; // The effect of each move
unsigned short dist[1 << 16];  // The distance cache
unsigned short q[1 << 16];     // Our queue for the bfs
int front = 0, back = 0;       // The queue positions

int main()
{
    std::memset(dist, ~0, sizeof(dist));

    // Calculate the move masks (behold the indentation stairs!!)
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            for(int di = -1; di <= 1; di++)
                for(int dj = -1; dj <= 1; dj++)
                    if(dj*di == 0)
                        if(i+di >= 0 && i+di < 4 && j+dj >= 0 && j+dj < 4)
                            mask[i+j*4] |= (1 << ((i+di) + (j+dj)*4));
    unsigned short start = 0;
    // Read the input
    for(int i = 0; i < 4; i++)
    {
        char input[5];
        scanf("%s", input);
        for(int j = 0; j < 4; j++)
            if(input[j] == 'b')
                start |= 1 << (i*4+j);
    }
    q[back++] = start;
    dist[start] = 0;
    // bfs
    while(front != back)
    {
        auto b = q[front++];
        if(b == 0 || b == (unsigned short) ~0)
        {
            printf("%hu", dist[b]);
            return 0;
        }
        for(int i = 0; i < 16; i++)
        {
            int m = b ^ mask[i];
            if(dist[m] > dist[b]+1)
            {
                q[back++] = m;
                dist[m] = dist[b]+1;
            }
        }
    }
    printf("Impossible");
}
