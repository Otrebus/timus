/* 1401. Gamers - http://acm.timus.ru/problem.aspx?num=1401
 *
 * Strategy:
 * Recursion - for any block of side 2^n where we need to make a hole at some position, we
 * recursively generate the four sub-blocks like so (in this case the hole is in the top right
 * block, and n is 3):
 *
 * A A A A D D D D
 * A A A A D D   D
 * A A A A D D D D
 * A A A   D D D D
 * B B B     C C C
 * B B B B C C C C
 * B B B B C C C C
 * B B B B C C C C
 *
 * That is, for the three sub-blocks not containing the hole, we generate blocks for them with holes
 * in their corners closest to the middle of the entire block. This hole is filled with a triangle
 * shape. For the sub-block containing the hole we recurse normally.
 * 
 * Performance:
 * O(2^(2*n)), runs the test suite in 0.28s using 1416KB memory.
 */

#include <stdio.h>

int m = 3, A[512][512];

void generate(int n, int x, int y, int hx, int hy)
{
    if(n == 2)
    {
        for(int i = 0; i < 2; i++)
            for(int j = 0; j < 2; j++)
                if(x+i != hx || y+j != hy)
                    A[x+i][y+j] = m++/3;
        return;
    }
    for(int i = 0; i < 2; i++)
        for(int j = 0; j < 2; j++)
            if(x + i*n/2 > hx || hx >= x + i*n/2+n/2 || y + j*n/2 > hy || hy >= y + j*n/2+n/2)
                A[x+n/2-1+i][y+n/2-1+j] = m++/3; // Middle triangle
    for(int i = 0; i < 2; i++)
        for(int j = 0; j < 2; j++)
            if(x + i*n/2 <= hx && hx < x + i*n/2+n/2 && y + j*n/2 <= hy && hy < y + j*n/2+n/2)
                generate(n/2, x + i*n/2, y + j*n/2, hx, hy); // Block contains the hole
            else // Does not contain the hole
                generate(n/2, x + i*n/2, y + j*n/2, x + n/2 - 1 + i, y + n/2 - 1 + j);
}

int main()
{
    int n, px, py;
    scanf("%d %d %d", &n, &px, &py);
    int s = [] (int n) { int i = 1; while(n--) i*=2; return i; }(n);
    generate(s, 0, 0, px-1, py-1);
    for(int i = 0; i < s; i++)
    {
        for(int j = 0; j < s; j++)
            printf("%d ", A[i][j]);
        printf("\n");
    }
}