/* 1525. Path - http://acm.timus.ru/problem.aspx?num=1525
 *
 * Strategy:
 * Treat each dimension separately and note that after first hitting a wall, the rest of the
 * sequence always plays out the same. We can thus vary the position we end up with by how
 * many ways we can not hit the wall at all, or, how many units further away from the maximum
 * extent of the input direction that we can move.
 *
 * Performance:
 * Linear in the size of the input, runs the test suite in 0.015s using 296KB memory.
 */

#include <stdio.h>
#include <algorithm>
 
char dir[128]; // To avoid switching, see below
char dim[128]; // Same
char input[100001];
int max[3], min[3]; // Furthest the input takes us in each direction forward and backwards
int p[3], a[3]; // p is used to update the above, a is the answer
int d[3]; // d is the size of the parallelogram
 
int main()
{
    dir['r'] = dir['u'] = dir['f'] = 1;
    dir['l'] = dir['d'] = dir['b'] = -1;
    dim['r'] = dim['l'] = 0;
    dim['u'] = dim['d'] = 1;
    dim['f'] = dim['b'] = 2;
    scanf("%d %d %d", d, d+1, d+2);
    scanf("%s", input);
    for(char* pc = input; *pc != 0; pc++)
    {
        char c = *pc;
        int D = dim[c];
        p[D] += dir[c];
        max[D] = std::max(max[D], p[D]);
        min[D] = std::min(min[D], p[D]);
    }
    for(int i = 0; i < 3; i++)
        a[i] = std::max((d[i]+min[i])-max[i], 1);
    printf("%lld\n", (long long)a[0]*(long long)a[1]*(long long)a[2]);
    return 0;
}